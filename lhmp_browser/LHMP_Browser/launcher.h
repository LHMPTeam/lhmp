#ifndef LAUNCHER_H
#define LAUNCHER_H

#define _WINSOCKAPI_

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QInputDialog>
#include <QGraphicsDropShadowEffect>
#include <QLineEdit>
#include <QKeyEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFileDialog>
#include <QTreeView>
#include <QSettings>
#include <VersionHelpers.h>

namespace Ui {
class Launcher;
}

class Launcher : public QMainWindow
{
    Q_OBJECT

public:
    explicit Launcher(QWidget *parent = 0);
    ~Launcher();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_btnPlay_clicked();

    void on_pushButton_6_clicked();

    void on_tableWidget_itemSelectionChanged();

    void replyFinished (QNetworkReply *reply);

    void on_pushButton_5_clicked();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void on_label_21_linkActivated(const QString &link);

    void on_pushButton_7_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_textEdited(const QString &arg1);

private:
    Ui::Launcher *ui;

    bool ReadConfig();
    bool WriteConfig();
    bool CheckForUpdates();
    bool RefreshServerList();
    bool AskPatchPermission();
    void PopulateServerList();
    bool JoinGame(QString address, QString password);
    void SetMafiaPath();
    void ShowServerInfo();
    QString FileChecksum(QString path);

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    QPoint pos;
    QNetworkAccessManager *manager;

    QString nickname, clientVersion, mafiaPath, filesURL, serversURL;

    QString launcherVersion = "1.0.5";
    QString filesPath = "lhmp/";
    QUrl launcherUpdateJSONURL = QUrl("http://lh-mp.eu/launcher/launcher.json");
};

#endif // LAUNCHER_H
