/***************************************************************************
                             qgsprojectproperties.h    
       Set various project properties (inherits qgsprojectpropertiesbase)
                              -------------------
  begin                : May 18, 2004
  copyright            : (C) 2004 by Gary E.Sherman
  email                : sherman at mrcc.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/* $Id: qgsprojectproperties.h 6447 2007-01-21 05:29:14Z telwertowski $ */

#include "ui_qgsprojectpropertiesbase.h"
#include "qgis.h"
#include "qgisgui.h"

class QgsMapCanvas;


/*!  Dialog to set project level properties

  @note actual state is stored in QgsProject singleton instance

 */
class QgsProjectProperties : public QDialog, private Ui::QgsProjectPropertiesBase
{
  Q_OBJECT
public:
    //! Constructor
  QgsProjectProperties(QgsMapCanvas* mapCanvas, QWidget *parent = 0, Qt::WFlags fl = QgisGui::ModalDialogFlags);

  //! Destructor
  ~QgsProjectProperties();

  /*! Gets the currently select map units
   */
  QGis::units mapUnits() const;

  /*!
   * Set the map units
   */
  void setMapUnits(QGis::units);

  /*!
     Every project has a title
  */
  QString title() const;
  void title( QString const & title );
  
  /*! Accessor for projection */
  QString projectionWKT();

  /*! Indicates that the projection switch is on */
  bool isProjected();

public slots:
  /*!
   * Slot called when apply button is pressed or dialog is accepted
   */
  void apply();

  /*!
   * Slot to show the projections tab when the dialog is opened
   */
  void showProjectionsTab();
  
  /*!
   * Slot to select the digitizing line colour
   */
  void on_pbnDigitisedLineColour_clicked();

  /*!
   * Slot to select the map selection colour
   */
  void on_pbnSelectionColour_clicked();
  
  /*!
   * Slot to select the map selection colour
   */
  void on_pbnCanvasColor_clicked();

  /*!
   * Slot to show the context help for this dialog
   */
  void on_buttonBox_helpRequested();

signals:
  //! Signal used to inform listeners that the mouse display precision may have changed
  void displayPrecisionChanged();

  //! let listening canvases know to refresh
  void refresh();
  

private:
  static const int context_id = 361087368;
  
  QgsMapCanvas* mMapCanvas;

};