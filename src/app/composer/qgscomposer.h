/***************************************************************************
                         qgscomposer.h
                             -------------------
    begin                : January 2005
    copyright            : (C) 2005 by Radim Blazek
    email                : blazek@itc.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/* $Id: qgscomposer.h 6415 2007-01-09 02:39:15Z wonder $ */
#ifndef QGSCOMPOSER_H
#define QGSCOMPOSER_H
#include "ui_qgscomposerbase.h"

class QgisApp;
class QgsComposerView;
class QgsComposition;
class QgsMapCanvas;

class QGridLayout;
class QPrinter;
class QDomNode;
class QDomDocument;
class QMoveEvent;
class QResizeEvent;
class QFile;
class QSizeGrip;

/* The constructor creates empty composer, without compositions and mFirstTime set to true. 
 * - if signal projectRead() is recieved all old compositions are deleted and
 *     - if the composition exists in project it is created from project settings (mFirstTime set to false)
 *     - if the composition does not exist in project 
 *         - if the composer is visible new default composition is created (mFirstTime set to false)
 *         - if the composer is not visible the composer is left empty (mFirstTime set to true)
 * - if signal newProject() is recieved all old compositions are deleted and 
 *     - if the composer is visible a new default composition is created (mFirstTime set to false)
 *     - if the composer is not visible the composer is left empty (mFirstTime set to true)
 *         
 * If open() is called and mFirstTime == true, a new default composition is created.
 *
 */
class QgsComposer: public QMainWindow, private Ui::QgsComposerBase
{
    Q_OBJECT

public:
    QgsComposer(QgisApp *qgis);
    ~QgsComposer();
    
    //! Open and show, set defaults if first time
    void open();

    //! Zoom to full extent of the paper
    void zoomFull();

    //! Select item
    void selectItem();

    //! Return pointer to map canvas
    QgsMapCanvas *mapCanvas( void );
    
    //! Return pointer to composer view
    QgsComposerView *view ( void );

    //! Return current composition
    QgsComposition *composition(void);

    //! Show composition options in widget
    void showCompositionOptions ( QWidget *w );
    
    //! Show item options in widget
    void showItemOptions ( QWidget *w );

    /** \brief stores statei in project */
    bool writeSettings ( void );

    /** \brief read state from project */
    bool readSettings ( void );

    //! Stores state in DOM node
    bool writeXML( QDomNode & node, QDomDocument & doc);

    //! Sets state from DOM document
    bool readXML( QDomNode & node );

    //! Restore the window and toolbar state
    void restoreWindowState();

    //! Move event
    void moveEvent ( QMoveEvent * );
    
    //! Resize event
    void resizeEvent ( QResizeEvent * );

public slots:
    //! Zoom to full extent of the paper
    void on_mActionZoomAll_activated(void);

    //! Zoom in
    void on_mActionZoomIn_activated(void);

    //! Zoom out 
    void on_mActionZoomOut_activated(void);

    //! Refresh view 
    void on_mActionRefreshView_activated(void);

    //! Print the composition
    void on_mActionPrint_activated(void);
    
    //! Print as image
    void on_mActionExportAsImage_activated(void);
    
    //! Print as SVG
    void on_mActionExportAsSVG_activated(void);
    
    //! Select item
    void on_mActionSelectMoveItem_activated(void);
    
    //! Add new map
    void on_mActionAddNewMap_activated(void);

    //! Add new vector legend
    void on_mActionAddNewVectLegend_activated(void);
    
    //! Add new label
    void on_mActionAddNewLabel_activated(void);
    
    //! Add new scalebar
    void on_mActionAddNewScalebar_activated(void);
    
    //! Add new picture
    void on_mActionAddImage_activated(void);

    //! read project
    void projectRead();

    //! New project
    void newProject();

    //! Save window state
    void saveWindowState();

    //! Slot for when the help button is clicked
    void on_helpPButton_clicked();

    //! Slot for when the close button is clicked
    void on_closePButton_clicked();

private:
    //! Set teh pixmap / icons on the toolbar buttons
    void setupTheme();
    //! remove widget childrens
    void removeWidgetChildren ( QWidget *w );

    /** \brief move up the content of the file
        \param file file
	\param from starting position
	\param shift shift in bytes 
    */
    bool shiftFileContent ( QFile *file, Q_LONG start, int shift );
    
    //! Set buttons up
    void setToolActionsOff (void);

    //! returns new world matrix for canvas view after zoom with factor scaleChange
    QMatrix updateMatrix(double scaleChange);
    
    //! Pointer to composer view
    QgsComposerView *mView;

    //! Current composition
    QgsComposition *mComposition;
    
    //! Printer
    QPrinter *mPrinter;

    //! Pointer to QGIS application
    QgisApp *mQgis;

    //! The composer was opened first time (-> set defaults)
    bool mFirstTime;

    //! Layout 
    QGridLayout *mCompositionOptionsLayout;

    //! Layout
    QGridLayout *mItemOptionsLayout;

    //! Size grip
    QSizeGrip *mSizeGrip;

    //! Help context id
    static const int context_id = 985715179;

};

#endif