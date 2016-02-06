/***************************************************************************
                          qgsproject.cpp -  description
                             -------------------
  begin                : July 23, 2004
  copyright            : (C) 2004 by Mark Coletti
  email                : mcoletti at gmail.com
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsproject.h"

#include <deque>
#include <memory>
#include <cassert>
#include <iostream>

#include "qgslogger.h"
#include "qgsrect.h"
#include "qgsvectorlayer.h"
#include "qgsrasterlayer.h"
#include "qgsmaplayerregistry.h"
#include "qgsexception.h"
#include "qgsprojectproperty.h"
#include "qgslogger.h"

#include <QApplication>
#include <QFileInfo>
#include <QDomNode>
#include <QObject>
#include <QTextStream>


static const char *const ident_ = "$Id: qgsproject.cpp 7122 2007-08-05 20:09:32Z wonder $";





// / canonical project instance
QgsProject * QgsProject::theProject_;







/**
    Take the given scope and key and convert them to a string list of key
    tokens that will be used to navigate through a Property hierarchy

    E.g., scope "someplugin" and key "/foo/bar/baz" will become a string list
    of { "properties", "someplugin", "foo", "bar", "baz" }.  "properties" is
    always first because that's the permanent ``root'' Property node.
 */
 static 
 QStringList makeKeyTokens_(QString const &scope, QString const &key)
 {
     // XXX - debugger probes
     //const char * scope_str = scope.toLocal8Bit().data();
     //const char * key_str   = key.toLocal8Bit().data();

     QStringList keyTokens = QStringList(scope);
     keyTokens += QStringList::split('/', key);

     // be sure to include the canonical root node
     keyTokens.push_front( "properties" );

     return keyTokens;
 } // makeKeyTokens_




 /**
    return the property that matches the given key sequence, if any

    @param sequence is a tokenized key sequence
    @param p is likely to be the top level QgsPropertyKey in QgsProject:e:Imp.

    @return null if not found, otherwise located Property
 */
 static
 QgsProperty * findKey_( QString const & scope, 
                         QString const & key, 
                         QgsPropertyKey &   rootProperty )
 {
     QgsPropertyKey * currentProperty = &rootProperty;
     QgsProperty * nextProperty;           // link to next property down hiearchy

     QStringList keySequence = makeKeyTokens_( scope, key );

     while ( ! keySequence.isEmpty() )
     {
         // if the current head of the sequence list matches the property name,
         // then traverse down the property hierarchy
         if ( keySequence.first() == currentProperty->name() )
         {
             // remove front key since we're traversing down a level
             keySequence.pop_front();

             // if we have only one key name left, then return the key found
             if ( 1 == keySequence.count() )
             {
                 return currentProperty->find( keySequence.front() );
                 
             }
             // if we're out of keys then the current property is the one we
             // want; i.e., we're in the rate case of being at the top-most
             // property node
             else if ( keySequence.isEmpty() )
             {
                 return currentProperty;
             }
             else if (( nextProperty = currentProperty->find( keySequence.first() ) ))
             {
                 if ( nextProperty->isKey() )
                 {
                     currentProperty = dynamic_cast<QgsPropertyKey*>(nextProperty);
                 }
                 // it may be that this may be one of several property value
                 // nodes keyed by QDict string; if this is the last remaining
                 // key token and the next property is a value node, then
                 // that's the situation, so return the currentProperty
                 else if ( nextProperty->isValue() && (1 == keySequence.count()) )
                 {
                     return currentProperty;
                 }
                 else            // QgsPropertyValue not Key, so return null
                 {
                     return 0x0;
                 }
             }
             else                // if the next key down isn't found
             {                   // then the overall key sequence doesn't exist
                 return 0x0;
             }
         }
         else
         {
             return 0x0;
         }
     }

     return 0x0;
 } // findKey_



 /** add the given key and value

 @param sequence is list of keys
 @param rootProperty is the property from which to start adding
 @param value the value associated with the key
 */
 static
 QgsProperty * addKey_( QString const & scope,
                        QString const & key, 
                        QgsPropertyKey * rootProperty,
                        QVariant value )
 {
     QStringList keySequence = makeKeyTokens_( scope, key );

     // cursor through property key/value hierarchy
     QgsPropertyKey * currentProperty = rootProperty;

     QgsProperty * newProperty; // link to next property down hiearchy

     while ( ! keySequence.isEmpty() )
     {
         // if the current head of the sequence list matches the property name,
         // then traverse down the property hierarchy
         if ( keySequence.first() == currentProperty->name() )
         {
             // remove front key since we're traversing down a level
             keySequence.pop_front();

             // if key sequence has one last element, then we use that as the
             // name to store the value
             if ( 1 == keySequence.count() )
             {
                 currentProperty->setValue( keySequence.front(), value );
                 return currentProperty;
             }
             // we're at the top element if popping the keySequence element
             // will leave it empty; in that case, just add the key
             else if ( keySequence.isEmpty() )
             {
                 currentProperty->setValue( value );

                 return currentProperty;
             }
             else if (( newProperty = currentProperty->find( keySequence.first() ) ))
             {
                 currentProperty = dynamic_cast<QgsPropertyKey*>(newProperty);

                 if ( currentProperty )
                 {
                     continue;
                 }
                 else            // QgsPropertyValue not Key, so return null
                 {
                     return 0x0;
                 }
             }
             else                // the next subkey doesn't exist, so add it
             {
                 newProperty = currentProperty->addKey( keySequence.first() );

                 if ( newProperty )
                 {
                     currentProperty = dynamic_cast<QgsPropertyKey*>(newProperty);
                 }
                 continue;
             }
         }
         else
         {
             return 0x0;
         }
     }

     return 0x0;

 } // addKey_



 static
 void removeKey_(QString const & scope,
                 QString const & key, 
                 QgsPropertyKey & rootProperty)
 {
     QgsPropertyKey * currentProperty = &rootProperty;

     QgsProperty * nextProperty = NULL;   // link to next property down hiearchy
     QgsPropertyKey * previousQgsPropertyKey = NULL; // link to previous property up hiearchy

     QStringList keySequence = makeKeyTokens_( scope, key );

     while ( ! keySequence.isEmpty() )
     {
         // if the current head of the sequence list matches the property name,
         // then traverse down the property hierarchy
         if ( keySequence.first() == currentProperty->name() )
         {
             // remove front key since we're traversing down a level
             keySequence.pop_front();

             // if we have only one key name left, then try to remove the key
             // with that name
             if ( 1 == keySequence.count() )
             {
                 currentProperty->removeKey( keySequence.front() );
             }
             // if we're out of keys then the current property is the one we
             // want to remove, but we can't delete it directly; we need to
             // delete it from the parent property key container
             else if ( keySequence.isEmpty() )
             {
                 previousQgsPropertyKey->removeKey( currentProperty->name() );
             }
             else if ( 0 == ( nextProperty = currentProperty->find( keySequence.first() ) ) )
             {
                 previousQgsPropertyKey = currentProperty;
                 currentProperty = dynamic_cast<QgsPropertyKey*>(nextProperty);

                 if ( currentProperty )
                 {
                     continue;
                 }
                 else            // QgsPropertyValue not Key, so return null
                 {
                     return;
                 }
             }
             else                // if the next key down isn't found
             {                   // then the overall key sequence doesn't exist
                 return;
             }
         }
         else
         {
             return;
         }
     }

 } // void removeKey_



 struct QgsProject::Imp
 {
     /// current physical project file
     QFile file;

     /// property hierarchy
     QgsPropertyKey properties_;

     /// project title
     QString title;

     /// true if project has been modified since it has been read or saved
     bool dirty;

     Imp()
         : title(""), 
           dirty(false)
     {                             // top property node is the root
                                   // "properties" that contains all plug-in
                                   // and extra property keys and values
         properties_.name() = "properties"; // root property node always this value
     }

     /** clear project properties when a new project is started 
      */ 
     void clear()
     {
         QgsDebugMsg( "Clearing project properties Impl->clear();" );

         properties_.clearKeys();
         title = "";

         // reset some default project properties
         // XXX THESE SHOULD BE MOVED TO STATUSBAR RELATED SOURCE
         QgsProject::instance()->writeEntry("PositionPrecision", "/Automatic", true);
         QgsProject::instance()->writeEntry("PositionPrecision", "/DecimalPlaces", 2);
     }

 };  // struct QgsProject::Imp



 QgsProject::QgsProject()
     : imp_(new QgsProject::Imp)
 {
     // Set some default project properties
     // XXX THESE SHOULD BE MOVED TO STATUSBAR RELATED SOURCE
     writeEntry("PositionPrecision", "/Automatic", true);
     writeEntry("PositionPrecision", "/DecimalPlaces", 2);
     // XXX writeEntry() makes the project dirty, but it doesn't make sense
     // for a new project to be dirty, so let's clean it up
     dirty(false);
 } // QgsProject ctor



 QgsProject::~QgsProject()
 {
     // note that std::auto_ptr automatically deletes imp_ when it's destroyed
 } // QgsProject dtor



 QgsProject * QgsProject::instance()
 {
     if (!QgsProject::theProject_)
     {
         QgsProject::theProject_ = new QgsProject;
     }

     return QgsProject::theProject_;
 } // QgsProject * instance()




 void QgsProject::title(QString const &title)
 {
     imp_->title = title;

     dirty(true);
 } // void QgsProject::title


 QString const & QgsProject::title() const
 {
     return imp_->title;
 } // QgsProject::title() const


 bool QgsProject::dirty() const
 {
     return imp_->dirty;
 } // bool QgsProject::dirty() 


 void QgsProject::dirty(bool b)
 {
     imp_->dirty = b;
 } // bool QgsProject::dirty()



 void QgsProject::filename(QString const &name)
 {
     imp_->file.setName(name);

     dirty(true);
 } // void QgsProject::filename( QString const & name )



 QString QgsProject::filename() const
 {
     return imp_->file.name();
 } // QString QgsProject::filename() const



 /// basically a debugging tool to dump property list values 
 static void dump_( QgsPropertyKey const & topQgsPropertyKey )
 {
     qDebug("current properties:");

     topQgsPropertyKey.dump();
 } // dump_




 /**

 Restore any optional properties found in "doc" to "properties".

 <properties> tags for all optional properties.  Within that there will be
 scope tags.  In the following example there exist one property in the
 "fsplugin" scope.  "layers" is a list containing three string values.

 <properties>
   <fsplugin>
     <foo type="int" >42</foo>
     <baz type="int" >1</baz>
     <layers type="QStringList" >
       <value>railroad</value>
       <value>airport</value>
     </layers>
     <xyqzzy type="int" >1</xyqzzy>
     <bar type="double" >123.456</bar>
     <feature_types type="QStringList" >
        <value>type</value>
     </feature_types>
   </fsplugin>
 </properties>

 @param project_properties should be the top QgsPropertyKey node.

 */
static 
void
_getProperties(QDomDocument const &doc, QgsPropertyKey & project_properties)
{
    QDomNodeList properties = doc.elementsByTagName("properties");

    if (properties.count() > 1)
    {
        qDebug("there appears to be more than one ``properties'' XML tag ... bailing");
        return;
    } else if (properties.count() < 1)  // no properties found, so we're done
    {
        return;
    }

    // item(0) because there should only be ONE
    // "properties" node
    QDomNodeList scopes = properties.item(0).childNodes();

    if (scopes.count() < 1)
    {
        qDebug("empty ``properties'' XML tag ... bailing");
        return;
    }

    QDomNode propertyNode = properties.item(0);

    if ( ! project_properties.readXML( propertyNode ) )
    {
        qDebug("Project_properties.readXML() failed");
    }

// DEPRECATED as functionality has been shoved down to QgsProperyKey::readXML()

//     size_t i = 0;
//     while (i < scopes.count())
//     {
//         QDomNode curr_scope_node = scopes.item(i);

//         qDebug("found %d property node(s) for scope %s",
//                curr_scope_node.childNodes().count(),
//                (const char *) curr_scope_node.nodeName().utf8());

//         QString key(curr_scope_node.nodeName());

//         QgsPropertyKey * currentKey = 
//             dynamic_cast<QgsPropertyKey*>(project_properties.find( key ));

//         if ( ! currentKey )
//         {
//             // if the property key doesn't yet exist, create an empty instance
//             // of that key

//             currentKey = project_properties.addKey( key );

//             if ( ! currentKey )
//             {
//                 qDebug( "%s:%d unable to add key", __FILE__, __LINE__ );
//             }
//         }

//         if (! currentKey->readXML(curr_scope_node))
//         {
//             qDebug("%s:%d unable to read XML for property %s", __FILE__, __LINE__,
//                    (const char *) curr_scope_node.nodeName().utf8());
//         }

//         ++i;
//     }
} // _getProperties




/**
   Get the project title

   XML in file has this form:
   <qgis projectname="default project">
   <title>a project title</title>

   @todo XXX we should go with the attribute xor <title>, not both.
*/
static void _getTitle(QDomDocument const &doc, QString & title)
{
    QDomNodeList nl = doc.elementsByTagName("title");

    title = "";                 // by default the title will be empty

    if (!nl.count())
    {
        qDebug("%s : %d %s", __FILE__, __LINE__, " unable to find title element\n");
        return;
    }

    QDomNode titleNode = nl.item(0);  // there should only be one, so zeroth element ok

    if (!titleNode.hasChildNodes()) // if not, then there's no actual text
    {
        qDebug("%s : %d %s", __FILE__, __LINE__, " unable to find title element\n");
        return;
    }

    QDomNode titleTextNode = titleNode.firstChild();  // should only have one child

    if (!titleTextNode.isText())
    {
        qDebug("%s : %d %s", __FILE__, __LINE__, " unable to find title element\n");
        return;
    }

    QDomText titleText = titleTextNode.toText();

    title = titleText.data();

} // _getTitle


/** return the version string found in the given DOM document

   @returns the version string or an empty string if none found
 */
static QString _getVersion(QDomDocument const &doc)
{
    QDomNodeList nl = doc.elementsByTagName("qgis");

    if (!nl.count())
    {
        QgsDebugMsg(" unable to find qgis element in project file");
        return "";
    }
    
    QDomNode qgisNode = nl.item(0);  // there should only be one, so zeroth element ok

    QDomElement qgisElement = qgisNode.toElement(); // qgis node should be element

    return qgisElement.attribute("version");
} // _getVersion



/**
   Read map layers from project file

   
   @returns pair of < bool, list<QDomNode> >; bool is true if function worked;
            else is false.  list contains nodes corresponding to layers that couldn't
            be loaded

   @note XML of form:

   <maplayer type="vector">
      <layername>Hydrop</layername>
      <datasource>/data/usgs/city_shp/hydrop.shp</datasource>
      <zorder>0</zorder>
      <provider>ogr</provider>
      <singlesymbol>
         <renderitem>
            <value>blabla</value>
            <symbol>
               <outlinecolor red="85" green="0" blue="255" />
               <outlinestyle>SolidLine</outlinestyle>
               <outlinewidth>1</outlinewidth>
               <fillcolor red="0" green="170" blue="255" />
               <fillpattern>SolidPattern</fillpattern>
            </symbol>
            <label>blabla</label>
         </renderitem>
      </singlesymbol>
      <label>0</label>
      <labelattributes>
         <label text="Label" field="" />
         <family name="Sans Serif" field="" />
         <size value="12" units="pt" field="" />
         <bold on="0" field="" />
         <italic on="0" field="" />
         <underline on="0" field="" />
         <color red="0" green="0" blue="0" field="" />
         <x field="" />
         <y field="" />
         <offset  units="pt" x="0" xfield="" y="0" yfield="" />
         <angle value="0" field="" />
         <alignment value="center" field="" />
      </labelattributes>
   </maplayer>

*/
static std::pair< bool, std::list<QDomNode> > _getMapLayers(QDomDocument const &doc)
{
    // Layer order is implicit in the order they are stored in the project file

    QDomNodeList nl = doc.elementsByTagName("maplayer");

    // XXX what is this used for? QString layerCount( QString::number(nl.count()) );

    QString wk;

    std::list<QDomNode> brokenNodes; // a list of DOM nodes corresponding to layers
                                // that we were unable to load; this could be
                                // because the layers were removed or
                                // re-located after the project was last saved

    // process the map layer nodes

    if (0 == nl.count())        // if we have no layers to process, bail
    {
        return make_pair(true, brokenNodes); // Decided to return "true" since it's
                                // possible for there to be a project with no
                                // layers; but also, more imporantly, this
                                // would cause the tests/qgsproject to fail
                                // since the test suite doesn't currently
                                // support test layers
    }

    bool returnStatus = true;

    for (int i = 0; i < nl.count(); i++)
    {
        QDomNode node = nl.item(i);
        QDomElement element = node.toElement();

        QString type = element.attribute("type");

        QgsDebugMsg("Layer type is " + type);

        QgsMapLayer *mapLayer = NULL;
        
        if (type == "vector")
        {
            mapLayer = new QgsVectorLayer;
        } else if (type == "raster")
        {
            mapLayer = new QgsRasterLayer;
        }

        Q_CHECK_PTR(mapLayer);

        if (!mapLayer)
        {
          QgsDebugMsg("Unable to create layer");
          
          return make_pair(false, brokenNodes);
        }

        // have the layer restore state that is stored in DOM node
        if ( mapLayer->readXML(node) )
        {
            mapLayer = QgsMapLayerRegistry::instance()->addMapLayer(mapLayer);
        }
        else
        {
            delete mapLayer;

            QgsDebugMsg("Unable to load " + type + " layer");

            returnStatus = false; // flag that we had problems loading layers

            brokenNodes.push_back( node );
        }
    }

    return make_pair(returnStatus, brokenNodes);

} // _getMapLayers




/**
   Please note that most of the contents were copied from qgsproject
*/
bool QgsProject::read(QFileInfo const &file)
{
    imp_->file.setName(file.filePath());

    return read();
} // QgsProject::read( QFile & file )



/**
   @note it's presumed that the caller has already reset the map canvas, map
   registry, and legend
*/
bool QgsProject::read()
{
    std::auto_ptr< QDomDocument > doc =
        std::auto_ptr < QDomDocument > (new QDomDocument("qgis"));

    if (!imp_->file.open(QIODevice::ReadOnly))
    {
        imp_->file.close();     // even though we got an error, let's make
                                // sure it's closed anyway

        throw QgsIOException( QObject::tr("Unable to open ") + imp_->file.name());

        return false;           // XXX raise exception? Ok now superfluous
                                // XXX because of exception.
    }

    // location of problem associated with errorMsg
    int line, column;
    QString errorMsg;

    if (!doc->setContent(&imp_->file, &errorMsg, &line, &column))
    {
        // want to make this class as GUI independent as possible; so commented out
        // QMessageBox::critical( 0x0, "Project File Read Error",
        // errorMsg + " at line " + QString::number( line ) +
        // " column " + QString::number( column ) );

        QString errorString = QObject::tr("Project file read error: ") +
            errorMsg + QObject::tr(" at line ") + QString::number(line) + QObject::tr(" column ") +
            QString::number(column);

        qDebug((const char *) errorString.utf8());

        imp_->file.close();

        throw QgsException(errorString + QObject::tr(" for file ") + imp_->file.name());

        return false;               // XXX superfluous because of exception
    }

    imp_->file.close();


    QgsDebugMsg("Opened document " + imp_->file.name());

    // before we start loading everything, let's clear out the current set of
    // properties first so that we don't have the properties from the previous
    // project still hanging around

    imp_->clear();

    // now get any properties
    _getProperties(*doc, imp_->properties_);

    QgsDebugMsg(QString::number(imp_->properties_.count()) + " properties read");

    dump_(imp_->properties_);


    // restore the canvas' area of interest

    // now get project title
    _getTitle(*doc, imp_->title);

    // get project version string, if any
    QString fileVersion = _getVersion(*doc);

    if ( fileVersion.isNull() )
    {
        QgsDebugMsg( "project file has no version string" );
    }
    else
    {
        QgsDebugMsg( "project file has version " + QString(fileVersion) );
    }

    QStringList fileVersionParts = fileVersion.split(".");
    QStringList qgisVersionParts = QString(QGis::qgisVersion).split(".");

    QgsDebugMsg("Project title: " + imp_->title);
    
    bool older = false;

    if (fileVersionParts.size() != 3 || qgisVersionParts.size() != 3)
      older = false; // probably an older version
    else
    {
      if (fileVersionParts.at(0) < qgisVersionParts.at(0))
        older = true;
      else if (fileVersionParts.at(1) < qgisVersionParts.at(1))
        older = true;
      else if (fileVersionParts.at(2) < qgisVersionParts.at(2))
        older = true;
    }

    if (older)
    {
      QgsLogger::warning("Loading a file that was saved with an older "
                         "version of qgis (saved in " + fileVersion +
                         ", loaded in " + QGis::qgisVersion +
                         "). Problems may occur.");
    }

    // get the map layers
    std::pair< bool, std::list<QDomNode> > getMapLayersResults =  _getMapLayers(*doc);

    // review the integrity of the retrieved map layers

    if ( ! getMapLayersResults.first )
    {
        QgsDebugMsg("Unable to get map layers from project file.");

        if ( ! getMapLayersResults.second.empty() )
        {
          QgsDebugMsg("there are " + QString::number(getMapLayersResults.second.size()) + " broken layers");
        }

        // Since we could be executing this from the test harness which
        // doesn't *have* layers -- nor a GUI for that matter -- we'll just
        // leave in the whining and boldly stomp on.
	emit readProject(*doc);
        throw QgsProjectBadLayerException( getMapLayersResults.second );

//         return false;
    }

    // read the project: used by map canvas and legend
    emit readProject(*doc);

    // can't be dirty since we're allegedly in pristine state
    dirty(false);

    return true;

} // QgsProject::read





bool QgsProject::read( QDomNode & layerNode )
{
    QString type = layerNode.toElement().attribute("type");

    QgsMapLayer *mapLayer;

    if (type == "vector")
    {
        mapLayer = new QgsVectorLayer;
    } 
    else if (type == "raster")
    {
        mapLayer = new QgsRasterLayer;
    }
    else
    {
        QgsDebugMsg("bad layer type");
        return false;
    }

    if (!mapLayer)
    {
        QgsDebugMsg("unable to create layer");
        return false;
    }

    // have the layer restore state that is stored in DOM node
    if ( mapLayer->readXML(layerNode) )
    {
        mapLayer = QgsMapLayerRegistry::instance()->addMapLayer(mapLayer);
    }
    else
    {
        delete mapLayer;

        QgsDebugMsg("unable to load " + type + " layer");

        return false;
    }

    return  true;
} // QgsProject::read( QDomNode & layerNode )



bool QgsProject::write(QFileInfo const &file)
{
    imp_->file.setName(file.filePath());

    return write();
} // QgsProject::write( QFileInfo const & file )


bool QgsProject::write()
{
  // if we have problems creating or otherwise writing to the project file,
  // let's find out up front before we go through all the hand-waving
  // necessary to create all the DOM objects
  if (!imp_->file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
  {
    imp_->file.close();         // even though we got an error, let's make
    // sure it's closed anyway

    throw QgsIOException(QObject::tr("Unable to save to file ") + imp_->file.name());

    return false;               // XXX raise exception? Ok now superfluous
    // XXX because of exception.
  }
  QFileInfo myFileInfo(imp_->file);
  if (!myFileInfo.isWritable())
  {
    // even though we got an error, let's make
    // sure it's closed anyway
    imp_->file.close();         
    throw QgsIOException(imp_->file.name() + QObject::tr(QString(" is not writeable.")
            + QString("Please adjust permissions (if possible) and try again.")));
    // XXX raise exception? Ok now superfluous
    return false;               
   
  }

  QDomImplementation DOMImplementation;

  QDomDocumentType documentType =
      DOMImplementation.createDocumentType("qgis", "http://mrcc.com/qgis.dtd",
              "SYSTEM");
  std::auto_ptr < QDomDocument > doc =
      std::auto_ptr < QDomDocument > (new QDomDocument(documentType));


  QDomElement qgisNode = doc->createElement("qgis");
  qgisNode.setAttribute("projectname", title());
  qgisNode.setAttribute("version", QString("%1").arg(QGis::qgisVersion));

  doc->appendChild(qgisNode);

  // title
  QDomElement titleNode = doc->createElement("title");
  qgisNode.appendChild(titleNode);

  QDomText titleText = doc->createTextNode(title());  // XXX why have title TWICE?
  titleNode.appendChild(titleText);
  
  // let map canvas and legend write their information
  emit writeProject(*doc);

  // within top level node save list of layers
  std::map<QString,QgsMapLayer*> & layers = QgsMapLayerRegistry::instance()->mapLayers();

  // Iterate over layers in zOrder
  // Call writeXML() on each
  QDomElement projectLayersNode = doc->createElement("projectlayers");
  projectLayersNode.setAttribute("layercount", qulonglong( layers.size() ));

  std::map<QString,QgsMapLayer*>::iterator li = layers.begin();
  while (li != layers.end())
  {
    //QgsMapLayer *ml = QgsMapLayerRegistry::instance()->mapLayer(*li);
    QgsMapLayer* ml = li->second;

    if (ml)
    {
      ml->writeXML(projectLayersNode, *doc);
    }
    li++;
  }

  qgisNode.appendChild(projectLayersNode);
  
  // now add the optional extra properties

  dump_(imp_->properties_);

  qDebug("there are %d property scopes", static_cast<int>(imp_->properties_.count()));

  if (!imp_->properties_.isEmpty()) // only worry about properties if we
    // actually have any properties
  {
    imp_->properties_.writeXML( "properties", qgisNode, *doc );
  } 

  // now wrap it up and ship it to the project file
  doc->normalize();             // XXX I'm not entirely sure what this does

  //QString xml = doc->toString(4); // write to string with indentation of four characters
  // (yes, four is arbitrary)

  // const char * xmlString = xml; // debugger probe point
  // qDebug( "project file output:\n\n" + xml );

  QTextStream projectFileStream(&imp_->file);

  //projectFileStream << xml << endl;
  doc->save(projectFileStream, 4);  // save as utf-8
  imp_->file.close();


  dirty(false);                 // reset to pristine state

  return true;
} // QgsProject::write



void QgsProject::clearProperties()
{
    QgsDebugMsg("Clearing project properties QgsProject::clearProperties()");
    
    imp_->clear();

    dirty(true);
} // QgsProject::clearProperties()



bool
QgsProject::writeEntry(QString const &scope, const QString & key, bool value)
{
    dirty(true);

    return addKey_( scope, key, &imp_->properties_, value );
} // QgsProject::writeEntry ( ..., bool value )


bool
QgsProject::writeEntry(QString const &scope, const QString & key,
                       double value)
{
    dirty(true);

    return addKey_( scope, key, &imp_->properties_, value );
} // QgsProject::writeEntry ( ..., double value )


bool
QgsProject::writeEntry(QString const &scope, const QString & key, int value)
{
    dirty(true);

    return addKey_( scope, key, &imp_->properties_, value );
} // QgsProject::writeEntry ( ..., int value )


bool
QgsProject::writeEntry(QString const &scope, const QString & key,
                       const QString & value)
{
    dirty(true);

    return addKey_( scope, key, &imp_->properties_, value );
} // QgsProject::writeEntry ( ..., const QString & value )


bool
QgsProject::writeEntry(QString const &scope, const QString & key,
                       const QStringList & value)
{
    dirty(true);

    return addKey_( scope, key, &imp_->properties_, value );
} // QgsProject::writeEntry ( ..., const QStringList & value )




QStringList
QgsProject::readListEntry(QString const & scope,
                          const QString & key,
                          bool * ok) const 
{
    QgsProperty * property = findKey_( scope, key, imp_->properties_ );

    QVariant value;

    if ( property )
    {
        value = property->value();
    }

    bool valid = QVariant::StringList == value.type();

    if (ok)
    {
        *ok = valid;
    }

    if (valid)
    {
        return value.asStringList();
    }

    return QStringList();
} // QgsProject::readListEntry


QString
QgsProject::readEntry(QString const & scope, 
                      const QString & key,
                      const QString & def,
                      bool * ok) const 
{
    QgsProperty * property = findKey_( scope, key, imp_->properties_ );

    QVariant value;

    if ( property )
    {
        value = property->value();
    }

    bool valid = value.canCast(QVariant::String);

    if (ok)
    {
        *ok = valid;
    }

    if (valid)
    {
        return value.toString();
    }

    return QString(def);
} // QgsProject::readEntry


int
QgsProject::readNumEntry(QString const &scope, const QString & key, int def,
                         bool * ok) const 
{
    QgsProperty * property = findKey_( scope, key, imp_->properties_ );

    QVariant value;

    if ( property )
    {
        value = property->value();
    }

    bool valid = value.canCast(QVariant::String);

    if (ok)
    {
        *ok = valid;
    }

    if (valid)
    {
        return value.toInt();
    }

    return def;
} // QgsProject::readNumEntry


double
QgsProject::readDoubleEntry(QString const &scope, const QString & key,
                            double def,
                            bool * ok) const 
{
    QgsProperty * property = findKey_( scope, key, imp_->properties_ );

    QVariant value;

    if ( property )
    {
        value = property->value();
    }

    bool valid = value.canCast(QVariant::Double);

    if (ok)
    {
        *ok = valid;
    }

    if (valid)
    {
        return value.toDouble();
    }

    return def;
} // QgsProject::readDoubleEntry


bool
QgsProject::readBoolEntry(QString const &scope, const QString & key, bool def,
                          bool * ok) const
{
    QgsProperty * property = findKey_( scope, key, imp_->properties_ );

    QVariant value;

    if ( property )
    {
        value = property->value();
    }

    bool valid = value.canCast(QVariant::Bool);

    if (ok)
    {
        *ok = valid;
    }

    if (valid)
    {
        return value.toBool();
    }

    return def;
} // QgsProject::readBoolEntry


bool QgsProject::removeEntry(QString const &scope, const QString & key)
{
    removeKey_( scope, key, imp_->properties_ );

    dirty(true);

    return ! findKey_(scope, key, imp_->properties_ );
} // QgsProject::removeEntry



QStringList QgsProject::entryList(QString const &scope, QString const &key) const
{
    QgsProperty * foundProperty = findKey_( scope, key, imp_->properties_ );

    QStringList entries;

    if ( foundProperty )
    {
        QgsPropertyKey * propertyKey = dynamic_cast<QgsPropertyKey*>(foundProperty);

        if (propertyKey)
        { propertyKey->entryList( entries ); }
    }

    return entries;
} // QgsProject::entryList 


QStringList 
QgsProject::subkeyList(QString const &scope, QString const &key) const
{
    QgsProperty * foundProperty = findKey_( scope, key, imp_->properties_ );

    QStringList entries;

    if ( foundProperty )
    {
        QgsPropertyKey * propertyKey = dynamic_cast<QgsPropertyKey*>(foundProperty);

        if (propertyKey)
        { propertyKey->subkeyList( entries ); }
    }

    return entries;

} // QgsProject::subkeyList 



void QgsProject::dumpProperties() const
{
    dump_(imp_->properties_);
} // QgsProject::dumpProperties