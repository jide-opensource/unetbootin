//! [0]
        QFile file;
        QDir::setCurrent("/tmp");
        file.setFileName("readme.txt");
        QDir::setCurrent("/home");
        file.open(QIODevice::ReadOnly);      // opens "/home/readme.txt" under Unix
//! [0]


//! [1]
        QByteArray myEncoderFunc(const QString &fileName);
//! [1]


//! [2]
        QString myDecoderFunc(const QByteArray &localFileName);
//! [2]


//! [3]
        #include <stdio.h>

        void printError(const char* msg)
        {
            QFile file;
            file.open(stderr, QIODevice::WriteOnly);
            file.write(msg, qstrlen(msg));        // write to stderr
            file.close();
        }
//! [3]


//! [4]
    CONFIG += console
//! [4]


