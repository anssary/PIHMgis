#ifndef TIMESERIES
#define TIMESERIES

#include "ui_timeseries.h"

class timeSeriesDlg : public QDialog, private Ui::timeSeriesDialog
{
	Q_OBJECT

public:
	timeSeriesDlg();

public slots:
	void browse();
	void plot();
	void savePlot();
	void help();

};

#endif