#include "launcher.h"
#include "ui_launcher.h"
#include "CLHMPQuery.h"

struct ServerInfo {
    int ID;
    short players, maxPlayer, ping;

    QString hostname;
    QString gamemode;
    QString address;
    QString map;

    QString playerList;

    bool password;
};

struct UpdateFile {
    QString name, hash;
};

CLHMPQuery* query;

ServerInfo queryResultServers[1024];
UpdateFile files[128];
int queryCount, queryCountMax, serverCount, playerCount, allowPatch = -1, needsUpdatingCount = 0, shouldUpdate = -1;
bool refreshing = false, playerlist = false, masterDown = false, noPlayers = false, patchQuestionRan = false, updated = false;

void queryCallback(unsigned int serverID, void* data, unsigned char reasonID)
{
    if (reasonID == QUERY_OVERALL) {
        OverallPacket* packet = (OverallPacket*)data;

        char* hostname;
        char* gamemode;

        hostname = packet->hostname;
        gamemode = packet->gamemode;

        queryResultServers[packet->ID].ID = packet->ID;
        queryResultServers[packet->ID].ping = packet->ping;
        queryResultServers[packet->ID].hostname = hostname;
        queryResultServers[packet->ID].gamemode = gamemode;
        queryResultServers[packet->ID].map = packet->mapname;
        queryResultServers[packet->ID].players = packet->players;
        queryResultServers[packet->ID].maxPlayer = packet->maxPlayer;
        queryResultServers[packet->ID].password = packet->hasPassword;

        queryCount++;
    } else if (reasonID == QUERY_PLAYERLIST){
        PlayerPacket* packet = (PlayerPacket*)data;

        queryResultServers[packet->ID].playerList = "";

        for (int i = 0; i < packet->playerCount; i++) {
            queryResultServers[packet->ID].playerList.append(packet->playersPool[i].nickname);
            queryResultServers[packet->ID].playerList.append("|");

            qDebug() << queryResultServers[packet->ID].playerList;
        }
    } else if (reasonID == QUERY_CONNECTION_FAILED) {
        qDebug() << QString("Connection failed on task: %1 ID[%2]\n").arg(reasonID).arg(serverID);

        queryCount++;
    } else if (reasonID == QUERY_MASTER_FAILED) {
        masterDown = true;
    } else if (reasonID == QUERY_MASTER_SUCCESS) {
        MasterResponse* packet = (MasterResponse*)data;

        queryCount = 0;
        queryCountMax = packet->count;

        for (int i = 0; i < packet->count; i++) {
            query->queryInfo(packet->servers[i].IP, packet->servers[i].port + 1, i);
            query->queryPlayerlist(packet->servers[i].IP, packet->servers[i].port + 1, i);

            queryResultServers[i].address = QString("%1:%2").arg(packet->servers[i].IP).arg(packet->servers[i].port);
        }
    }
}

Launcher::Launcher(QWidget *parent) : QMainWindow(parent), ui(new Ui::Launcher) {
    ui->setupUi(this);

    // Create network manager
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    // Add validator to nickname input boxz
    QRegExp regex("^[A-Za-z0-9 _]*[A-Za-z0-9][A-Za-z0-9 _]*$");
    QRegExpValidator *validator = new QRegExpValidator(regex);

    ui->lineEdit->setValidator(validator);

    // Set default nickname
    nickname = QDir::home().dirName();
    ui->lineEdit->setText(nickname);

    // Window style, remove borders
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // Shadow for window frame
    QGraphicsDropShadowEffect* eShadow = new QGraphicsDropShadowEffect();

    QColor c = Qt::black;
    c.setAlpha(150);

    eShadow->setBlurRadius(15);
    eShadow->setColor(c);
    eShadow->setXOffset(0);
    eShadow->setYOffset(0);

    ui->frame->setGraphicsEffect(eShadow);

    // Button for server info / playerlist
    ui->label_21->setVisible(false);
    ui->label_21->setText("<a href=\"#\" style=\"color: #fff; border: none; outline: none;\">Show player list</a>");
    ui->label_21->setTextFormat(Qt::RichText);
    ui->label_21->setTextInteractionFlags(Qt::TextBrowserInteraction);

    // Shadows for title
    QColor cBlack = Qt::black;

    QGraphicsDropShadowEffect* eShadowTitle = new QGraphicsDropShadowEffect();

    eShadowTitle->setColor(cBlack);
    eShadowTitle->setXOffset(1);
    eShadowTitle->setYOffset(1);

    ui->label_5->setGraphicsEffect(eShadowTitle);

    // Table settings
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableWidget->setColumnWidth(0, 335);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 50);
    ui->tableWidget->setColumnWidth(3, 50);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    ui->tableWidget->horizontalHeader()->setHighlightSections(false);

    ui->tableWidget->setColumnHidden(4, true);
    ui->tableWidget->setColumnHidden(5, true);
    ui->tableWidget->setColumnHidden(6, true);
    ui->tableWidget->setColumnHidden(7, true);

    // 2nd table settings
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableWidget_2->setColumnWidth(0, 75);
    ui->tableWidget_2->setColumnWidth(1, 150);

    // Disable play button
    ui->btnPlay->setEnabled(false);

    // Start query instance
    query = CLHMPQuery::getInstance();
    query->Prepare(&queryCallback, 1000);

    // Fade in
    QPropertyAnimation *aHide = new QPropertyAnimation(this, "windowOpacity");
    aHide->setDuration(0);
    aHide->setStartValue(0);
    aHide->setEndValue(0);
    aHide->start();

    QPropertyAnimation *aFadeIn = new QPropertyAnimation(this, "windowOpacity");
    aFadeIn->setDuration(100);
    aFadeIn->setStartValue(0.0);
    aFadeIn->setEndValue(1.0);
    aFadeIn->setEasingCurve(QEasingCurve::Linear);
    aFadeIn->start();

    ReadConfig();

    if (mafiaPath.isEmpty()) {
        SetMafiaPath();
    } else {
        // Check Game.exe exists
        QString pathExecutable = mafiaPath;
                pathExecutable.append("/Game.exe");

        QFile file(pathExecutable);

        if (file.exists()) {
            CheckForUpdates();
        } else {
            SetMafiaPath();
        }

        file.close();
    }
}

Launcher::~Launcher()
{
    delete ui;
}

void Launcher::PopulateServerList() {
    int rowCount = 0;

    serverCount = 0;

    while (ui->tableWidget->rowCount() > 0) {
        ui->tableWidget->removeRow(0);
    }

    for (int i = 0; i < 1024; i++) {
        if (!queryResultServers[i].hostname.isEmpty()) {
            rowCount = ui->tableWidget->rowCount();

            ui->tableWidget->insertRow(rowCount);

            playerCount += queryResultServers[i].players;

            QString players = QString("%1 / %2").arg(queryResultServers[i].players).arg(queryResultServers[i].maxPlayer);

            ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(queryResultServers[i].hostname));
            ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(queryResultServers[i].gamemode));
            ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(players));
            ui->tableWidget->setItem(rowCount, 3, new QTableWidgetItem(QString::number(queryResultServers[i].ping)));
            ui->tableWidget->setItem(rowCount, 4, new QTableWidgetItem(queryResultServers[i].address));
            ui->tableWidget->setItem(rowCount, 5, new QTableWidgetItem(queryResultServers[i].map));
            ui->tableWidget->setItem(rowCount, 6, new QTableWidgetItem(queryResultServers[i].playerList));

            QString password = "No";
            if (queryResultServers[i].password) password = "Yes";

            ui->tableWidget->setItem(rowCount, 7, new QTableWidgetItem(password));

            ui->tableWidget->item(rowCount, 2)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(rowCount, 3)->setTextAlignment(Qt::AlignCenter);

            serverCount++;

            ui->label_10->setText(QString::number(serverCount));
            ui->label_12->setText(QString::number(playerCount));
        }
    }

    if (serverCount == 0) {
        rowCount = ui->tableWidget->rowCount();

        ui->tableWidget->insertRow(rowCount);

        if (masterDown) {
            ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem("Error contacting the master server, please try again later."));
        } else {
            ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem("No servers found."));

            noPlayers = true;
        }
    }

    refreshing = false;
}

void Launcher::replyFinished(QNetworkReply *reply)
{
    if(!reply->error()) {
        // launcher.json file
        if (reply->url() == launcherUpdateJSONURL) {
            QString version, url;
            QJsonValue value;
            QJsonObject item;

            QString response = reply->readAll();

            QJsonDocument document = QJsonDocument::fromJson(response.toUtf8());
            QJsonObject object = document.object();

            // Check launcher version
            value = object.value("launcher");
            item = value.toObject();

            version = item["version"].toString();
            url = item["url"].toString();
            filesURL = item["files"].toString();
            serversURL = item["servers"].toString();

            if (launcherVersion != version) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "New update available", "There is a new update available!<br/><br/>Do you want to download it and update your installation?", QMessageBox::Yes|QMessageBox::No);

                if (reply == QMessageBox::Yes) {
                    // Self update
                    manager->get(QNetworkRequest(QString("%1setup.exe").arg(filesURL)));

                    ui->label_3->setText("Downloading new launcher update...");
                } else {
                    ui->label_3->setText("You are running an outdated version.");
                }
            } else {
                ui->label_3->setText("You are running the latest version.");
            }

            // Check client version
            value = object.value("client");
            item = value.toObject();

            version = item["version"].toString();
            ui->label_8->setText(version);

            // List of launcher files
            value = object.value("files");
            QJsonArray files = value.toArray(), fileDetails;

            QString name, hash, hashLocal, path;

            path = QString("%1/%2").arg(mafiaPath).arg(filesPath);

            if (!QDir(path).exists()) {
                QDir().mkdir(path);
            }

            foreach (const QJsonValue & v, files) {
                fileDetails = v.toArray();

                name = fileDetails[0].toString();
                hash = fileDetails[1].toString();

                QString location = QString("%1%2").arg(path).arg(name);

                if (name == "loader.exe" || name == "Game.exe" || name == "LS3DF.dll" || name == "CrashHandler.exe") {
                    location = QString("%1/%2").arg(mafiaPath).arg(name);
                }

                hashLocal = FileChecksum(location);

                // Download and update
                qDebug() << hashLocal.isEmpty() << (hashLocal != hash);

                if (hashLocal.isEmpty() || hashLocal != hash) {
                    if (shouldUpdate == -1) {
                        QMessageBox::StandardButton reply;
                        reply = QMessageBox::question(this, "New update available", "There is a new update available!<br/><br/>Do you want to download it and update your installation?", QMessageBox::Yes|QMessageBox::No);

                        if (reply == QMessageBox::Yes) {
                            shouldUpdate = 1;
                        } else {
                            shouldUpdate = 0;
                        }
                    }

                    if (shouldUpdate == 1) {
                        manager->get(QNetworkRequest(QString("%1%2").arg(filesURL).arg(name)));

                        needsUpdatingCount++;

                        qDebug() << location << name << hashLocal << hash << QString("%1%2").arg(filesURL).arg(name);
                    }
                }
            }

            if (needsUpdatingCount == 0) {
                updated = true;
            } else {
                ui->label_3->setText("Downloading new updates...");
            }

            //debug
            //updated = true;

            RefreshServerList();
        } else {
            // File dl
            QString fileName = reply->url().toString();
            fileName.replace(filesURL, "");

            // If launcher
            if (fileName == "setup.exe") {
                QString path = QString("%1/setup.exe").arg(QDir::tempPath());

                QFile *file = new QFile(path);

                if(file->open(QFile::WriteOnly)) {
                    file->write(reply->readAll());

                    file->flush();
                    file->close();
                }

                delete file;

                // Start update process
                path = QDir::currentPath();

                QProcess *process = new QProcess();

                process->startDetached(QString("%1/setup.exe").arg(QDir::tempPath()));

                exit(0);
            } else if (fileName == "Game.exe") {
                if (!QFile(QString("%1/%2.bk").arg(mafiaPath).arg(fileName)).exists()) {
                    if (allowPatch == -1) {
                        AskPatchPermission();
                    }

                    if (allowPatch == 1) {
                        QFile::copy(QString("%1/%2").arg(mafiaPath).arg(fileName), QString("%1/%2.bk").arg(mafiaPath).arg(fileName));

                        QString path = QString("%1/%2").arg(mafiaPath).arg(fileName);

                        QFile *file = new QFile(path);

                        if(file->open(QFile::WriteOnly)) {
                            file->write(reply->readAll());

                            file->flush();
                            file->close();
                        }

                        delete file;

                        needsUpdatingCount--;
                    }
                }
            } else if (fileName == "LS3DF.dll") {
                if (!QFile(QString("%1/%2.bk").arg(mafiaPath).arg(fileName)).exists()) {
                    if (allowPatch == -1) {
                        AskPatchPermission();
                    }

                    if (allowPatch == 1) {
                        QFile::copy(QString("%1/%2").arg(mafiaPath).arg(fileName), QString("%1/%2.bk").arg(mafiaPath).arg(fileName));

                        QString path = QString("%1/%2").arg(mafiaPath).arg(fileName);

                        QFile *file = new QFile(path);

                        if(file->open(QFile::WriteOnly)) {
                            file->write(reply->readAll());

                            file->flush();
                            file->close();
                        }

                        delete file;

                        needsUpdatingCount--;
                    }
                }
            } else if (fileName == "loader.exe") {
                QString path = QString("%1/%2").arg(mafiaPath).arg(fileName);

                QFile *file = new QFile(path);

                if(file->open(QFile::WriteOnly)) {
                    file->write(reply->readAll());

                    file->flush();
                    file->close();
                }

                delete file;

                needsUpdatingCount--;
            } else {
                QString path = QString("%1/%2/%3").arg(mafiaPath).arg(filesPath).arg(fileName);

                QFile *file = new QFile(path);

                if(file->open(QFile::WriteOnly)) {
                    file->write(reply->readAll());

                    file->flush();
                    file->close();
                }

                delete file;

                needsUpdatingCount--;
            }
        }
    } else {
        ui->label_3->setText("Could not contact the update server, visit lh-mp.eu for support.");

        updated = true;

        qDebug() << reply->errorString();
    }

    qDebug() << reply->url();

    if (needsUpdatingCount == 0) {
        updated = true;

        if (shouldUpdate == 1) {
            ui->label_3->setText("You are running the latest version.");
        } else if (shouldUpdate == 0) {
            ui->label_3->setText("You are running an outdated version.");
        }
    }

    reply->deleteLater();
}

bool Launcher::AskPatchPermission() {
    if (patchQuestionRan) return 0;

    patchQuestionRan = true;

    QMessageBox msg;
    msg.setWindowTitle("Lost Heaven Multiplayer");
    msg.setText("You must have Mafia version 1.0 installed in order to play Lost Heaven Multiplayer.");
    msg.setInformativeText("Do you want to install the needed version? A backup of yoru original files will be kept.");
    msg.setIcon(QMessageBox::Question);
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Ok);

    int ret = msg.exec();

    if (ret == QMessageBox::Cancel) {
        allowPatch = 0;

        updated = true;
    } else {
        allowPatch = 1;
    }

    return ret;
}

bool Launcher::ReadConfig() {
    /*QString path = QCoreApplication::applicationDirPath();
            path.append("/lhmp_config");

    QFile *file = new QFile(path);

    if (file->exists()) {
        file->open(QFile::ReadOnly);

        QString config = file->readAll();

        QJsonDocument document = QJsonDocument::fromJson(config.toUtf8());
        QJsonObject item = document.object();

        // Read name
        nickname = item["nickname"].toString();

        ui->lineEdit->setText(nickname);

        // Read mafia path
        mafiaPath = item["path"].toString();

        file->close();
    } else {
        WriteConfig();
    }

    return file->error();*/

    QSettings settings("Lost Heaven Multiplayer", "Launcher");

    nickname = settings.value("nickname", "").toString();

    if (!nickname.isEmpty()) {
        ui->lineEdit->setText(nickname);
    }

    mafiaPath = settings.value("path", "").toString();

    return true;
}

bool Launcher::WriteConfig() {
    /*QString path = QCoreApplication::applicationDirPath();
            path.append("/lhmp_config");

    QFile file(path);

    QJsonDocument document;
    QJsonObject item;

    // Write name
    nickname = ui->lineEdit->text();
    item["nickname"] = nickname;

    // Write mafia path
    item["path"] = mafiaPath;
    document.setObject(item);

    file.open(QFile::WriteOnly);
    file.write(document.toJson());
    file.flush();
    file.close();

    return file.error();*/

    nickname = ui->lineEdit->text();

    QSettings settings("Lost Heaven Multiplayer", "Launcher");

    settings.setValue("nickname", nickname);
    settings.setValue("path", mafiaPath);

    return true;
}

bool Launcher::CheckForUpdates() {
    manager->get(QNetworkRequest(launcherUpdateJSONURL));

    ui->label_3->setText("Checking for updates...");

    return true;
}

QString Launcher::FileChecksum(QString path) {
    QFile file(path);

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray fileData = file.readAll();
        QByteArray hash = QCryptographicHash::hash(fileData, QCryptographicHash::Md5);

        return QString::fromStdString(hash.toHex().toStdString());
    }

    return "";
}

bool Launcher::RefreshServerList() {
    if (!serversURL.isEmpty() && !refreshing) {
        refreshing = true;
        playerlist = false;
        playerCount = 0;

        ui->btnPlay->setEnabled(false);

        for (int i = 0; i < 1024; i++) {
            queryResultServers[i].hostname = "";
        }

        queryCountMax = -1;

        masterDown = false;
        noPlayers = false;

        query->queryMasterlist(serversURL.toUtf8());

        // Clear list
        while (ui->tableWidget->rowCount() > 0) {
            ui->tableWidget->removeRow(0);
        }

        while (ui->tableWidget_2->rowCount() > 0) {
            ui->tableWidget_2->removeRow(0);
        }

        ui->label_10->setText("0");
        ui->label_12->setText("0");

        ui->frame_13->setStyleSheet("#frame_13 {border-bottom: 1px solid rgba(0, 0, 0, 150); background: url(:/data/unknown.png);}");

        ui->label_6->setText("Unknown");
        ui->label_19->setText("Unknown");

        ui->label_22->setVisible(false);
        ui->label_23->setVisible(false);
        ui->line_7->setVisible(false);
        ui->label_21->setVisible(false);

        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowCount);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem("Refreshing server list..."));

        // Clever non-blocking sleep from Qt forum
        while (queryCountMax == -1 && !masterDown) {
            QEventLoop loop;

            QTimer::singleShot(100, &loop, SLOT(quit()));

            loop.exec();
        };

        while (queryCount < queryCountMax) {
            QEventLoop loop;

            QTimer::singleShot(100, &loop, SLOT(quit()));

            loop.exec();
        };

        PopulateServerList();

        return true;
    };

    return false;
}

void Launcher::on_pushButton_2_clicked()
{
    WriteConfig();

    QApplication::quit();
}

void Launcher::on_pushButton_3_clicked()
{
    showMinimized();
}

void Launcher::mousePressEvent( QMouseEvent *e ) {
    pos = e->pos();
}

void Launcher::mouseMoveEvent( QMouseEvent *e ) {
    move(e->globalPos() - pos);
}

void Launcher::on_pushButton_4_clicked()
{
    QDesktopServices::openUrl(QUrl("http://lh-mp.eu/forum/"));
}

void Launcher::on_btnPlay_clicked()
{
    QString password = ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 7)->text();

    if (password == "Yes") {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Password"), tr("Password:"), QLineEdit::Normal, "", &ok);

        if (ok && !text.isEmpty()) JoinGame(ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 4)->text(), text);
    } else {
        JoinGame(ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 4)->text(), "");
    }
}

void Launcher::on_pushButton_6_clicked()
{
    bool result;
    QString address = QInputDialog::getText(this, tr("Connect to server"), tr("Server address to connect to (IP:PORT):"), QLineEdit::Normal, tr("127.0.0.1:27015"), &result);

    if (result && !address.isEmpty() && updated) {
        JoinGame(address, "");
    }
}

void Launcher::SetMafiaPath() {
    bool loop = true;

    QMessageBox msg;
    msg.setWindowTitle("Lost Heaven Multiplayer");
    msg.setText("You need to specify the location of your Mafia installation folder in order to play Lost Heaven Multiplayer.<br/><br/><b>If you encounter any problems, you may need to run this launcher as an administrator.</b>");
    msg.setInformativeText("Please specify the location of your Mafia installation in the next window.");
    msg.setIcon(QMessageBox::Warning);
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Ok);

    int ret = msg.exec();

    if (ret == QMessageBox::Cancel) {
        QApplication::quit();

        exit(0);
    }

    QString path, pathExecutable;

    while (loop) {
        path = QFileDialog::getExistingDirectory(this, tr("Select Mafia Directory"), "C:/Program Files/");

        if (!path.isEmpty()) {
            pathExecutable = path;
            pathExecutable.append("/Game.exe");

            QFile file(pathExecutable);

            if (file.exists()) {
                 mafiaPath = path;

                 loop = false;

                 CheckForUpdates();
            } else {
                msg.setWindowTitle("Lost Heaven Multiplayer");
                msg.setText("Could not find the Mafia executable in the specified folder.");
                msg.setInformativeText("Please choose a new folder and try again.");
                msg.setIcon(QMessageBox::Critical);
                msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                msg.setDefaultButton(QMessageBox::Ok);
                msg.activateWindow();

                ret = msg.exec();

                if (ret == QMessageBox::Cancel) {
                    QApplication::quit();

                    exit(0);
                }
            }

            file.close();
        } else {
            QApplication::quit();

            exit(0);
        }
    }
}

bool Launcher::JoinGame(QString address, QString password) {
    if (address.isEmpty()) return false;

    qDebug() << password;

    QStringList params = address.split(":");

    if (!password.isEmpty()) params << password;

    QString pathExecutable = mafiaPath;
            pathExecutable.append("/Game.exe");

    // TODO remove filesPath if loader is launched from mafia root
    QString pathLoader = QString("%1/loader.exe").arg(mafiaPath).arg(filesPath);

    QFile fileExecutable(pathExecutable);
    QFile fileLoader(pathLoader);

    bool ok = fileExecutable.exists() && fileLoader.exists();

    if (ok) {
        QProcess *process = new QProcess(this);
        process->setWorkingDirectory(mafiaPath);

        process->start(pathLoader, params);

        qDebug() << process;
    } else {
        QMessageBox::critical(this, tr("Lost Heaven Multiplayer"), tr("Unable to launch Mafia and/or launcher executable."));
    }

    fileExecutable.close();
    fileLoader.close();

    return ok;
}

void Launcher::ShowServerInfo() {
    while (ui->tableWidget_2->rowCount() > 0) {
        ui->tableWidget_2->removeRow(0);
    }

    if (serverCount > 0 && !refreshing) {
        QString name, gamemode, players, address, ping, map, playerList, password;
        int rowCount = 0;

        if (updated) ui->btnPlay->setEnabled(true);
        ui->label_22->setVisible(true);
        ui->label_23->setVisible(true);
        ui->line_7->setVisible(true);

        name = ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 0)->text();
        gamemode = ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 1)->text();
        players = ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 2)->text();
        ping = QString("%1ms").arg(ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 3)->text());
        address = ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 4)->text();
        map = ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 5)->text();
        playerList = ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 6)->text();
        password = ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 7)->text();

        if (playerlist) {
            ui->tableWidget_2->setColumnWidth(0, 25);
            ui->tableWidget_2->setColumnWidth(1, 200);

            QStringList playersListSplit = playerList.split("|");

            for (int i = 0; i < (playersListSplit.count() - 1); i++) {
                ui->tableWidget_2->insertRow(rowCount + i);
                ui->tableWidget_2->setItem(rowCount + i, 0, new QTableWidgetItem(QString("#%1").arg(i + 1)));
                ui->tableWidget_2->setItem(rowCount + i, 1, new QTableWidgetItem(playersListSplit[i]));
            };

            if (playersListSplit.count()) {
                ui->tableWidget_2->setColumnWidth(0, 150);

                ui->tableWidget_2->insertRow(rowCount);
                ui->tableWidget_2->setItem(rowCount, 0, new QTableWidgetItem("No players to show."));
            }
        } else {
            ui->tableWidget_2->setColumnWidth(0, 75);
            ui->tableWidget_2->setColumnWidth(1, 150);

            rowCount = ui->tableWidget_2->rowCount();
            ui->tableWidget_2->insertRow(rowCount);
            ui->tableWidget_2->setItem(rowCount, 0, new QTableWidgetItem("Name"));
            ui->tableWidget_2->setItem(rowCount, 1, new QTableWidgetItem(name));

            rowCount = ui->tableWidget_2->rowCount();
            ui->tableWidget_2->insertRow(rowCount);
            ui->tableWidget_2->setItem(rowCount, 0, new QTableWidgetItem("Gamemode"));
            ui->tableWidget_2->setItem(rowCount, 1, new QTableWidgetItem(gamemode));

            rowCount = ui->tableWidget_2->rowCount();
            ui->tableWidget_2->insertRow(rowCount);
            ui->tableWidget_2->setItem(rowCount, 0, new QTableWidgetItem("Players"));
            ui->tableWidget_2->setItem(rowCount, 1, new QTableWidgetItem(players));

            rowCount = ui->tableWidget_2->rowCount();
            ui->tableWidget_2->insertRow(rowCount);
            ui->tableWidget_2->setItem(rowCount, 0, new QTableWidgetItem("Address"));
            ui->tableWidget_2->setItem(rowCount, 1, new QTableWidgetItem(address));

            rowCount = ui->tableWidget_2->rowCount();
            ui->tableWidget_2->insertRow(rowCount);
            ui->tableWidget_2->setItem(rowCount, 0, new QTableWidgetItem("Password"));
            ui->tableWidget_2->setItem(rowCount, 1, new QTableWidgetItem(password));
       }

        ui->label_22->setText(ping);
        ui->label_23->setText(ping);

        if (map == "freeride" || map.contains("mesto", Qt::CaseInsensitive) || map == "EXTREME" || map == "mise03-morello") {
            ui->frame_13->setStyleSheet("#frame_13 {border-bottom: 1px solid rgba(0, 0, 0, 150); background: url(:/data/city.png);}");

            ui->label_6->setText("Lost Heaven (City)");
            ui->label_19->setText("Lost Heaven (City)");
        } else if (map == "freekrajina" || map.contains("krajina", Qt::CaseInsensitive)) {
            ui->frame_13->setStyleSheet("#frame_13 {border-bottom: 1px solid rgba(0, 0, 0, 150); background: url(:/data/countryside.png);}");

            ui->label_6->setText("Lost Heaven (Countryside)");
            ui->label_19->setText("Lost Heaven (Countryside)");
        } else if (map == "mise06-autodrom") {
            ui->frame_13->setStyleSheet("#frame_13 {border-bottom: 1px solid rgba(0, 0, 0, 150); background: url(:/data/race.png);}");

            ui->label_6->setText("Lost Heaven Racing Circuit");
            ui->label_19->setText("Lost Heaven Racing Circuit");
        } else {
            ui->frame_13->setStyleSheet("#frame_13 {border-bottom: 1px solid rgba(0, 0, 0, 150); background: url(:/data/unknown.png);}");

            ui->label_6->setText("Unknown");
            ui->label_19->setText("Unknown");
        }

        ui->label_21->setVisible(true);
    }
}

void Launcher::on_tableWidget_itemSelectionChanged()
{

    ShowServerInfo();
}

void Launcher::on_pushButton_5_clicked()
{
    Launcher::RefreshServerList();
}

void Launcher::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    if (!masterDown && !noPlayers && updated) {
        QString password = ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 7)->text();

        if (password == "Yes") {
            bool ok;
            QString text = QInputDialog::getText(this, tr("Password"), tr("Password:"), QLineEdit::Normal, "", &ok);

            if (ok && !text.isEmpty()) JoinGame(ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 4)->text(), text);
        } else {
            JoinGame(ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 4)->text(), "");
        }
    }
}

void Launcher::on_label_21_linkActivated(const QString &link)
{
    playerlist = !playerlist;

    if (playerlist) {
        ui->label_21->setText("<a href=\"#\" style=\"color: #fff; border: none; outline: none;\">Show server info</a>");
    } else {
        ui->label_21->setText("<a href=\"#\" style=\"color: #fff; border: none; outline: none;\">Show player list</a>");
    }

    ShowServerInfo();
}
