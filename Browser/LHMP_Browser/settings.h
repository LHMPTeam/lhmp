// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QInputDialog>
#include <QGraphicsDropShadowEffect>
#include <QLineEdit>
#include <QFileDialog>
#include <QTreeView>
#include <QSettings>

#pragma pack(1)

#define padding(name,x) unsigned char name[x];

struct RegistryStruct
{
    padding(_unk1, 2);
    unsigned int width;
    unsigned int heigh;
    unsigned short bitdept;
    padding(_unk2,6);
    // The level of anti aliasing  = 0, 2x, 4x
    unsigned char antiAliasing;
    padding(_unk3, 2)
    bool	isFullscreenOn;
    padding(_unk4, 7)
    bool	isSoundMuted;
};
#pragma pop()

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private slots:
    void on_pushButton_2_clicked();

    void on_btnPlay_clicked();

    void on_btnPlay_2_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::Settings *ui;

    QPoint pos;

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    bool WriteConfig(int width, int height, int window);

    bool GetMafiaRegistryStructure(RegistryStruct* output);
    bool SetMafiaRegistryStructure(RegistryStruct* input);

};

#endif // SETTINGS_H
