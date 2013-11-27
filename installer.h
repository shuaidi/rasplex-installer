#ifndef INSTALLER_H
#define INSTALLER_H

#include <QDialog>
#include <QtXml>
#include <QNetworkAccessManager>
#include <QCryptographicHash>

#include "downloadmanager.h"

class DiskWriter;
class ConfigHandler;

namespace Ui {
class Installer;
}

class Installer : public QDialog
{
    Q_OBJECT
    
public:
    explicit Installer(QWidget *parent = 0);
    ~Installer();
    
private:
    Ui::Installer *ui;

    QXmlSimpleReader xmlReader;
    DownloadManager* manager;

    void parseAndSetLinks(const QByteArray &data);
    void saveAndUpdateProgress(QNetworkReply *reply);
    void reset();
    void disableControls();
    bool isChecksumValid();

    QByteArray rangeByteArray(qlonglong first, qlonglong last);
    QNetworkRequest createRequest(QUrl &url, qlonglong first, qlonglong last);
    unsigned int getUncompressedImageSize();
    void setImageFileName(QString filename);

    enum {
        RESPONSE_OK = 200,
        RESPONSE_PARTIAL = 206,
        RESPONSE_FOUND = 302,
        RESPONSE_REDIRECT = 307,
        RESPONSE_BAD_REQUEST = 400,
        RESPONSE_NOT_FOUND = 404
    };
    enum {
        STATE_IDLE,
        STATE_GETTING_LINKS,
        STATE_GETTING_URL,
        STATE_DOWNLOADING_IMAGE,
        STATE_WRITING_IMAGE
    } state;

    qlonglong bytesDownloaded;
    QString imageFileName;
    QCryptographicHash imageHash;
    QFile imageFile;
    QUrl downloadUrl;
    DiskWriter *diskWriter;
    ConfigHandler *configHandler;
    bool isCancelled;

private slots:
    void handleFinishedDownload(const QByteArray& data);
    void handlePartialData(const QByteArray& data, qlonglong total);
    void cancel();
    void updateLinks();
    void refreshDeviceList();
    void downloadImage();
    void getImageFileNameFromUser();
    void writeImageToDevice();
    void selectVideoOutput();
};

#endif // INSTALLER_H
