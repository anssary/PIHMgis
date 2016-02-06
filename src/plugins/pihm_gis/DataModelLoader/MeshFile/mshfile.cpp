#include <QtGui>
#include "mshfile.h"

#include <iomanip>
#include <fstream>
using namespace std;

#include "../../pihmLIBS/pickGridValue.h"
#include "../../pihmLIBS/helpDialog/helpdialog.h"

mshFileDlg::mshFileDlg(QWidget *parent)
{
	setupUi(this);
	connect(eleFileButton, SIGNAL(clicked()), this, SLOT(eleBrowse()));
	connect(nodeFileButton, SIGNAL(clicked()), this, SLOT(nodeBrowse()));
	connect(neighFileButton, SIGNAL(clicked()), this, SLOT(neighBrowse()));
	connect(sElevFileButton, SIGNAL(clicked()), this, SLOT(sEleBrowse()));
	connect(bElevFileButton, SIGNAL(clicked()), this, SLOT(bEleBrowse()));
	connect(mshFileButton, SIGNAL(clicked()), this, SLOT(mshBrowse()));
	connect(runButton, SIGNAL(clicked()), this, SLOT(run()));
	connect(helpButton, SIGNAL(clicked()), this, SLOT(help()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

void mshFileDlg::eleBrowse()
{
	QString str = QFileDialog::getOpenFileName(this, "Choose File", "~/","ele File(*.ele *.ELE)");
	eleFileLineEdit->setText(str);
}

void mshFileDlg::nodeBrowse()
{
        QString str = QFileDialog::getOpenFileName(this, "Choose File", "~/","node File(*.node *.NODE)");
        nodeFileLineEdit->setText(str);
}

void mshFileDlg::neighBrowse()
{
        QString str = QFileDialog::getOpenFileName(this, "Choose File", "~/","neigh File(*.neigh *.NEIGH)");
        neighFileLineEdit->setText(str);
}

void mshFileDlg::sEleBrowse()
{
        QString str = QFileDialog::getOpenFileName(this, "Choose File", "~/","Surface Elev File(*.adf *.ADF)");
        sElevFileLineEdit->setText(str);
}

void mshFileDlg::bEleBrowse()
{
        QString str = QFileDialog::getOpenFileName(this, "Choose File", "~/","Bed Elev File(*.adf *.ADF)");
        bElevFileLineEdit->setText(str);
}

void mshFileDlg::mshBrowse()
{
	QString temp = QFileDialog::getSaveFileName(this, "Choose File", "~/","mesh File(*.mesh *.MESH)");
	QString tmp = temp;
	if(!(tmp.toLower()).endsWith(".mesh")){
        	tmp.append(".mesh");
        	temp = tmp;
        }
	mshFileLineEdit->setText(temp);
}

void mshFileDlg::run()
{

	QString logFileName("/tmp/log.html");
	ofstream log;
	log.open(logFileName.ascii());
	log<<"<html><body><font size=3 color=black><p> Verifying Files...</p></font></body></html>";
        log.close();
        MessageLog->setSource(logFileName);
        MessageLog->setFocus();
        MessageLog->setModified(TRUE);

	ifstream ele, node, neigh, sele, bele;
        ofstream mesh;

        ele.open((eleFileLineEdit->text()).ascii());
        node.open((nodeFileLineEdit->text()).ascii());
        neigh.open((neighFileLineEdit->text()).ascii());
        sele.open((sElevFileLineEdit->text()).ascii());
        bele.open((bElevFileLineEdit->text()).ascii());
        mesh.open((mshFileLineEdit->text()).ascii(), ios::out);

	int runFlag = 1;
/*
        if(ele==NULL){
        	qWarning("\n%s doesn't exist!", (eleFileLineEdit->text()).ascii());
	        runFlag=0;
        }
	if(node==NULL){
                qWarning("\n%s doesn't exist!", (nodeFileLineEdit->text()).ascii());
                runFlag=0;
        }
	if(neigh==NULL){
                qWarning("\n%s doesn't exist!", (neighFileLineEdit->text()).ascii());
                runFlag=0;
        }
	if(sele==NULL){
                qWarning("\n%s doesn't exist!", (sElevFileLineEdit->text()).ascii());
                runFlag=0;
        }
	if(bele==NULL){
                qWarning("\n%s doesn't exist!", (bElevFileLineEdit->text()).ascii());
                runFlag=0;
        }
	if(mesh==NULL){
                qWarning("\n Please Enter mesh file name");
                runFlag=0;
        }
*/

	log.open(logFileName.ascii(), ios::app);
	if((eleFileLineEdit->text()).length()==0){
		log<<"<p><font size=3 color=red> Error! Please input .ele Input File</p>";
		runFlag = 0;
	}
	else{
		log<<"<p>Checking... "<<(eleFileLineEdit->text()).ascii()<<"... ";
		if(ele == NULL){
			log<<"<font size=3 color=red> Error!</p>";
			//qWarning("\n%s doesn't exist!", (inputFileLineEdit->text()).ascii());
			runFlag = 0;
		}
		else
			log<<"Done!</p>";
	}
	log.close();
	MessageLog->reload();
	QApplication::processEvents();

	
	log.open(logFileName.ascii(), ios::app);
	if((nodeFileLineEdit->text()).length()==0){
		log<<"<p><font size=3 color=red> Error! Please input .node Input File</p>";
		runFlag = 0;
	}
	else{
		log<<"<p>Checking... "<<(nodeFileLineEdit->text()).ascii()<<"... ";
		if(ele == NULL){
			log<<"<font size=3 color=red> Error!</p>";
			//qWarning("\n%s doesn't exist!", (inputFileLineEdit->text()).ascii());
			runFlag = 0;
		}
		else
			log<<"Done!</p>";
	}
	log.close();
	MessageLog->reload();
	QApplication::processEvents();

	
	log.open(logFileName.ascii(), ios::app);
	if((neighFileLineEdit->text()).length()==0){
		log<<"<p><font size=3 color=red> Error! Please input .neigh Input File</p>";
		runFlag = 0;
	}
	else{
		log<<"<p>Checking... "<<(neighFileLineEdit->text()).ascii()<<"... ";
		if(neigh == NULL){
			log<<"<font size=3 color=red> Error!</p>";
			//qWarning("\n%s doesn't exist!", (inputFileLineEdit->text()).ascii());
			runFlag = 0;
		}
		else
			log<<"Done!</p>";
	}
	log.close();
	MessageLog->reload();
	QApplication::processEvents();


	log.open(logFileName.ascii(), ios::app);
	if((sElevFileLineEdit->text()).length()==0){
		log<<"<p><font size=3 color=red> Error! Please input Surface Elev. Input File</p>";
		runFlag = 0;
	}
	else{
		log<<"<p>Checking... "<<(sElevFileLineEdit->text()).ascii()<<"... ";
		if(sele == NULL){
			log<<"<font size=3 color=red> Error!</p>";
			//qWarning("\n%s doesn't exist!", (inputFileLineEdit->text()).ascii());
			runFlag = 0;
		}
		else
			log<<"Done!</p>";
	}
	log.close();
	MessageLog->reload();
	QApplication::processEvents();


	log.open(logFileName.ascii(), ios::app);
	if((bElevFileLineEdit->text()).length()==0){
		log<<"<p><font size=3 color=red> Error! Please input Bed Elev. Input File</p>";
		runFlag = 0;
	}
	else{
		log<<"<p>Checking... "<<(bElevFileLineEdit->text()).ascii()<<"... ";
		if(bele == NULL){
			log<<"<font size=3 color=red> Error!</p>";
			//qWarning("\n%s doesn't exist!", (inputFileLineEdit->text()).ascii());
			runFlag = 0;
		}
		else
			log<<"Done!</p>";
	}
	log.close();
	MessageLog->reload();
	QApplication::processEvents();


	log.open(logFileName.ascii(), ios::app);
	if((mshFileLineEdit->text()).length()==0){
		log<<"<p><font size=3 color=red> Error! Please input .mesh Output File</p>";
		runFlag = 0;
	}
	else{
		log<<"<p>Checking... "<<(mshFileLineEdit->text()).ascii()<<"... ";
		if(mesh == NULL){
			log<<"<font size=3 color=red> Error!</p>";
			qWarning("\nCan not open output file name");
			runFlag = 0;
		}
		else
			log<<"Done!</p>";
	}
	log.close();
	MessageLog->reload();
	QApplication::processEvents();


	if(runFlag == 1){

		log.open(logFileName.ascii(), ios::app);
		log<<"<p>Running...";
		log.close();
		MessageLog->reload();
		QApplication::processEvents();

		GDALDataset *sElev, *bElev;
                double sRanges[6], bRanges[6];
                GDALAllRegister();
                sElev = (GDALDataset *)GDALOpen((sElevFileLineEdit->text()).ascii(), GA_ReadOnly);
                bElev = (GDALDataset *)GDALOpen((bElevFileLineEdit->text()).ascii(), GA_ReadOnly);
                getExtent(sElev, sRanges);
                getExtent(bElev, bRanges);


                int temp;
                int numEle, numNode, numNeigh;
                ele>>numEle; ele>>temp; ele>>temp;
                node>>numNode; node>>temp; node>>temp; node>>temp;
                neigh>>numNeigh; neigh>>temp;

                mesh<<numEle<<"\t"<<numNode;
                int index;
                int node0, node1, node2, nabr0, nabr1, nabr2;
                for(int i=0; i<numEle; i++){
                        ele>>index; ele>>node0; ele>>node1; ele>>node2;
                        neigh>>temp; neigh>>nabr0; neigh>>nabr1; neigh>>nabr2;
                        mesh<<"\n"<<index<<"\t"<<node0<<"\t"<<node1<<"\t"<<node2<<"\t"<<(nabr0<0?0:nabr0)<<"\t"<<(nabr1<0?0:nabr1)<<"\t"<<(nabr2<0?0:nabr2);
                }
                double X, Y, Zmin, Zmax;
                for(int i=0; i<numNode; i++){
                        node>>index; node>>X; node>>Y; node>>temp;
                        Zmin = getRasterValue(bElev, 1, X, Y, bRanges);
                        Zmax = getRasterValue(sElev, 1, X, Y, sRanges);
                        mesh<<"\n"<<index<<"\t"<<setprecision(20)<<X<<"\t"<<setprecision(20)<<Y<<"\t"<<setprecision(15)<<Zmin<<"\t"<<setprecision(15)<<Zmax;
                }

		log.open(logFileName.ascii(), ios::app);
		log<<" Done!</p>";
		log.close();
		MessageLog->reload();
		QApplication::processEvents();

	}	
}
			
void mshFileDlg::help()
{
	helpDialog* hlpDlg = new helpDialog(this, "Mesh File", 1, "helpFiles/meshfile.html", "Help :: Mesh File");
	hlpDlg->show();	
}