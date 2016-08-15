// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include <Windows.h>
#include <cstdio>
#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) : QDialog(parent), ui(new Ui::Settings) {
    ui->setupUi(this);

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

    // Shadows for title
    QColor cBlack = Qt::black;

    QGraphicsDropShadowEffect* eShadowTitle = new QGraphicsDropShadowEffect();

    eShadowTitle->setColor(cBlack);
    eShadowTitle->setXOffset(1);
    eShadowTitle->setYOffset(1);

    ui->label_5->setGraphicsEffect(eShadowTitle);

    ui->comboBox->addItem("Fullscreen");
    ui->comboBox->addItem("Fullscreen (Windowed)");
    ui->comboBox->addItem("Window");

    QSettings settings("Lost Heaven Multiplayer", "Launcher");

    int index = settings.value("screen", 0).toInt();

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

    RegistryStruct data;
    if (GetMafiaRegistryStructure(&data)) {
        ui->spinBox->setValue(data.width);
        ui->spinBox_2->setValue(data.heigh);

        if (data.isFullscreenOn && index != 1) index = 0;
    } else {
        // fail
    }

    ui->comboBox->setCurrentIndex(index);

    if (index == 1) {
        ui->spinBox->setValue(GetSystemMetrics(SM_CXSCREEN));
        ui->spinBox_2->setValue(GetSystemMetrics(SM_CYSCREEN));

        ui->spinBox->setEnabled(false);
        ui->spinBox_2->setEnabled(false);
    } else {
        ui->spinBox->setEnabled(true);
        ui->spinBox_2->setEnabled(true);
    }
}

bool Settings::WriteConfig(int width, int height, int window) {
    // 22. fscreen or not

    HKEY key;
    HRESULT res = RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Illusion Softworks\\Mafia"), &key);

    if (res != ERROR_SUCCESS) {
        return false;
    } else {
        DWORD size = 126;
        BYTE setup[126];
        DWORD type = REG_BINARY;

        res = RegQueryValueEx(key, TEXT("LS3D_setup"), NULL, &type, (LPBYTE)&setup, &size);

        if (res == ERROR_SUCCESS) {
            DWORD width = setup[3];
            width |= (setup[2] << 8);

            qDebug() << setup[3] << setup[2] << " = " << width;
        } else {
            return false;
        }

        RegCloseKey(key);

        return true;
    }
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_pushButton_2_clicked()
{
    close();
}

void Settings::on_btnPlay_clicked()
{
    RegistryStruct data;
    if (GetMafiaRegistryStructure(&data)) {
        // Overwrite data with 640x480 resolution values
        data.width = (int)ui->spinBox->value();
        data.heigh = (int)ui->spinBox_2->value();

        if (ui->comboBox->currentIndex() == 0) {
            data.isFullscreenOn = true;
        } else {
            data.isFullscreenOn = false;
        }

        SetMafiaRegistryStructure(&data);
    } else {
        // fail
    }

    QSettings settings("Lost Heaven Multiplayer", "Launcher");

    settings.setValue("screen", ui->comboBox->currentIndex());

    close();
}

void Settings::on_btnPlay_2_clicked()
{
    close();
}

void Settings::mousePressEvent( QMouseEvent *e ) {
    pos = e->pos();
}

void Settings::mouseMoveEvent( QMouseEvent *e ) {
    move(e->globalPos() - pos);
}

//Returns true on SUCCESS and fills output instance with data
bool Settings::GetMafiaRegistryStructure(RegistryStruct* output)
{
    HKEY key;
    HRESULT res = RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("Software\\Illusion Softworks\\Mafia"), &key);

    if (res == ERROR_SUCCESS) {
        DWORD size = 126;
        BYTE setup[126];
        DWORD type = REG_BINARY;

        res = RegQueryValueEx(key, TEXT("LS3D_setup"), NULL, &type, (LPBYTE)&setup, &size);

        if (res == ERROR_SUCCESS) {
            // if registry buffer has been successfully filled, fill up our output with data
            memcpy(output, setup, sizeof(RegistryStruct));
            RegCloseKey(key);
            return true;
        }
        RegCloseKey(key);
    }
    return false;
}

//Returns true on SUCCESS and fills output instance with data
bool Settings::SetMafiaRegistryStructure(RegistryStruct* input)
{
    HKEY key;
    HRESULT res = RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("Software\\Illusion Softworks\\Mafia"), &key);

    if (res == ERROR_SUCCESS) {
        DWORD size = 126;
        BYTE setup[126];
        DWORD type = REG_BINARY;

        res = RegQueryValueEx(key, TEXT("LS3D_setup"), NULL, &type, (LPBYTE)&setup, &size);

        if (res == ERROR_SUCCESS) {
            // if registry buffer has been successfully filled, fill up our output with data
            memcpy(setup, input, sizeof(RegistryStruct));
            res = RegSetValueEx(key, TEXT("LS3D_setup"), NULL,type, (LPBYTE)&setup,size);
            RegCloseKey(key);
            if (res == ERROR_SUCCESS)
                return true;
            return false;
        }
        RegCloseKey(key);
    }
    return false;
}

void Settings::on_comboBox_currentIndexChanged(int index)
{
    if (index == 1) {
        ui->spinBox->setValue(GetSystemMetrics(SM_CXSCREEN));
        ui->spinBox_2->setValue(GetSystemMetrics(SM_CYSCREEN));

        ui->spinBox->setEnabled(false);
        ui->spinBox_2->setEnabled(false);
    } else {
        ui->spinBox->setEnabled(true);
        ui->spinBox_2->setEnabled(true);
    }
}
