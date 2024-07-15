#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"
#include <QTableWidget>
#include <QString>
int numberOfDirectories = 0;
int numberOfFiles = 0;
int numberOfPermission = 0;
struct passwd *user = NULL;
string *directoriesArray;
string *filesArray;
string* permissionArray;
string previousDirectory;
string currentDirectory;
int depthDir = 0;
//using namespace std;
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    directoriesArray = new string[1];
    filesArray = new string[1];
    previousDirectory = startPage(previousDirectory, directoriesArray, filesArray, permissionArray, numberOfDirectories, numberOfFiles, numberOfPermission);
    currentDirectory = previousDirectory;

   outputData(ui);


}

void outputData(Ui::MainWindow *ui){

    ui->tableWidget->setColumnCount(2);

    ui->tableWidget->setRowCount(numberOfDirectories+numberOfFiles);
    ui->tableWidget->setColumnWidth(0, 1000);
    ui->tableWidget->setColumnWidth(1, 400);

    for(int ridx = 0;ridx< numberOfDirectories+numberOfFiles-1 ;ridx++){

             QTableWidgetItem *item = new QTableWidgetItem();
             item->setText(" ");
             ui->tableWidget->setItem(ridx,0, item);

     }
    for(int ridx = 0;ridx< numberOfDirectories+numberOfFiles-1 ;ridx++){

             QTableWidgetItem *item = new QTableWidgetItem();
             item->setText(" ");
             ui->tableWidget->setItem(ridx,1, item);

     }





    for(int ridx = 0;ridx< numberOfDirectories ;ridx++){
             QString qstr = QString::fromStdString(directoriesArray[ridx]);
             QTableWidgetItem *item = new QTableWidgetItem();
             item->setText(qstr);
             ui->tableWidget->setItem(ridx,0, item);

     }
   int counter = 0;
    for (int i = numberOfDirectories; i < numberOfDirectories + numberOfFiles; i++){
        QString qstr = QString::fromStdString(filesArray[counter]);
        counter++;
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(qstr);
        ui->tableWidget->setItem(i,0, item);
    }
    counter =0;
    for(int i=0;i<numberOfDirectories+numberOfFiles;i++){
        QString qstr = QString::fromStdString(permissionArray[counter]);

        counter++;
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(qstr);
        ui->tableWidget->setItem(i, 1, item);
    }
    ui->lineEdit_2->setText(QString::fromStdString(currentDirectory));
    ui->lineEdit->clear();
    if(depthDir ==0){
        ui->knopka->setDisabled(true);
    }
    else {
        ui->knopka->setEnabled(true);
    }
}





MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

    QString qstr = ui->lineEdit->text();
    string str = qstr.toStdString();
    if (str == ""){
        return;
    }
    depthDir = depthDir+1;
    string searchDirectory =  currentDirectory + "/" +  str;
    previousDirectory = currentDirectory;
    currentDirectory = searchDirectory;
    numberOfDirectories =0;
    numberOfFiles = 0;
    numberOfPermission = 0;
    directoryBrowsing(searchDirectory,directoriesArray, filesArray, permissionArray, numberOfDirectories, numberOfFiles, numberOfPermission);
    outputData(ui);
}

void MainWindow::on_knopka_clicked()
{
    depthDir--;
    numberOfDirectories =0;
    numberOfFiles = 0;
    numberOfPermission = 0;
    int i = currentDirectory.size();
    char* temp = (char*)calloc(currentDirectory.size()+1, sizeof(char));
    strcpy(temp, currentDirectory.c_str());


    while(temp[i]!= '/'){
        temp[i] = '\0';
        i--;
    }
    temp[i] = '\0';
    currentDirectory = string(temp);

    directoryBrowsing(currentDirectory,directoriesArray, filesArray, permissionArray, numberOfDirectories, numberOfFiles, numberOfPermission);
    outputData(ui);
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    if(column == 1){
        return;
    }
    if (row > numberOfDirectories-1){
        return;
    }
    depthDir++;
    numberOfDirectories =0;
    numberOfFiles = 0;
    numberOfPermission = 0;
    QTableWidgetItem *qitem = ui->tableWidget->item(row, column);
    QString qstr = qitem->text();
    string str = qstr.toStdString();
    currentDirectory = str;
    directoryBrowsing(str,directoriesArray, filesArray, permissionArray, numberOfDirectories, numberOfFiles, numberOfPermission);
    outputData(ui);

}
