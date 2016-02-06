/***************************************************************************
 *   Copyright (C) 2005 by Lars Luthman
 *   larsl@users.sourceforge.net
 *                                                                         *
 *   This is a plugin generated from the QGIS plugin template              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "plugingui.h"
#include "qgsleastsquares.h"
#include "qgspointdialog.h"
#include "qgsrasterlayer.h"
#include "qgsmaplayerregistry.h"
#include "qgsproject.h"

//qt includes
#include <QFileDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>

//standard includes

QgsGeorefPluginGui::QgsGeorefPluginGui() : QgsGeorefPluginGuiBase()
{
  
}

QgsGeorefPluginGui::QgsGeorefPluginGui(QgisInterface* theQgisInterface,
                                       QWidget* parent, Qt::WFlags fl)
  : QDialog(parent, fl), mIface(theQgisInterface)
{
  setupUi(this);
}  


QgsGeorefPluginGui::~QgsGeorefPluginGui()
{
}


void QgsGeorefPluginGui::on_pbnClose_clicked()
{
 close(1);
}


void QgsGeorefPluginGui::on_pbnSelectRaster_clicked() {
  QSettings settings;
  QString dir = settings.readEntry("/Plugin-GeoReferencer/rasterdirectory");
  if (dir.isEmpty())
    dir = ".";
  QString filename = 
    QFileDialog::getOpenFileName(this,
				 tr("Choose a raster file"),
                 dir,
				 tr("Raster files (*.*)"));
  leSelectRaster->setText(filename);
}


void QgsGeorefPluginGui::on_pbnEnterWorldCoords_clicked() {

  // Is there a filename
  if (leSelectRaster->text().isEmpty())
  {
    QMessageBox::critical(this, tr("Error"), 
			  tr("You need to specify a file to georeference first."));

    return;
  }
  // do we think that this is a valid raster?
  if (!QgsRasterLayer::isValidRasterFileName(leSelectRaster->text())) {
    QMessageBox::critical(this, tr("Error"), 
			  tr("The selected file is not a valid raster file."));
    return;
  }
  
  // remember the directory
  {
    QSettings settings;
    QFileInfo fileInfo(leSelectRaster->text());
    settings.writeEntry("/Plugin-GeoReferencer/rasterdirectory", 
			fileInfo.dirPath());
  }
  
  // guess the world file name
  QString raster = leSelectRaster->text();
  int point = raster.findRev('.');
  QString worldfile;
  if (point != -1 && point != raster.length() - 1) {
    worldfile = raster.left(point + 1);
    worldfile += raster.at(point + 1);
    worldfile += raster.at(raster.length() - 1);
    worldfile += 'w';
  }
  
  // check if there already is a world file
  if (!worldfile.isEmpty()) {
    if (QFile::exists(worldfile)) {
      QMessageBox::StandardButton r = QMessageBox::question(this, tr("World file exists"),
                       tr("<p>The selected file already seems to have a ")+
                       tr("world file! Do you want to replace it with the ")+
		               tr("new world file?</p>"),
                       QMessageBox::Ok | QMessageBox::Cancel);
      if (r == QMessageBox::Cancel)
        return;
      else
        QFile::remove(worldfile);
    }
  }
  
  QgsPointDialog* dlg = new QgsPointDialog(raster, mIface, this);
  dlg->show();
}