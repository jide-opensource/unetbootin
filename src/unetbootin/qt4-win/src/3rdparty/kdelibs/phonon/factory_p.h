/*  This file is part of the KDE project
    Copyright (C) 2004-2007 Matthias Kretz <kretz@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

*/

#ifndef PHONON_FACTORY_P_H
#define PHONON_FACTORY_P_H

#include "phonon/phonon_export.h"

#include <QtCore/QObject>
#include <QtCore/QStringList>

QT_BEGIN_HEADER
QT_BEGIN_NAMESPACE

class QUrl;
class QIcon;

namespace Phonon
{
    class PlatformPlugin;
    class MediaNodePrivate;
    class AbstractMediaStream;

/**
 * \internal
 * \brief Factory to access the preferred Backend.
 *
 * This class is used internally to get the backend's implementation.
 * It keeps track of the objects that were created. When a
 * request for a backend change comes, it asks all frontend objects to delete
 * their backend objects and then checks whether they were all deleted. Only
 * then the old backend is unloaded and the new backend is loaded.
 *
 * \author Matthias Kretz <kretz@kde.org>
 */
namespace Factory
{
    /**
     * Emits signals for Phonon::Factory.
     */
    class Sender : public QObject
    {
        Q_OBJECT
        Q_SIGNALS:
            /**
             * Emitted after the backend has successfully been changed.
             */
            void backendChanged();

            /**
             * \copydoc BackendCapabilities::Notifier::availableAudioOutputDevicesChanged
             */
            void availableAudioOutputDevicesChanged();
    };

    /**
     * Returns a pointer to the object emitting the signals.
     *
     * \see Sender::backendChanged()
     */
    PHONON_EXPORT Sender *sender();

    /**
     * Create a new backend object for a MediaObject.
     *
     * \return a pointer to the MediaObject the backend provides.
     */
    QObject *createMediaObject(QObject *parent = 0);
    /**
     * Create a new backend object for a Effect.
     *
     * \return a pointer to the Effect the backend provides.
     */
    QObject *createEffect(int effectId, QObject *parent = 0);
    /**
     * Create a new backend object for a VolumeFaderEffect.
     *
     * \return a pointer to the VolumeFaderEffect the backend provides.
     */
    QObject *createVolumeFaderEffect(QObject *parent = 0);
    /**
     * Create a new backend object for a AudioOutput.
     *
     * \return a pointer to the AudioOutput the backend provides.
     */
    QObject *createAudioOutput(QObject *parent = 0);
    /**
     * Create a new backend object for a AudioDataOutput.
     *
     * \return a pointer to the AudioDataOutput the backend provides.
     */
    PHONON_EXPORT QObject *createAudioDataOutput(QObject *parent = 0);
    /**
     * Create a new backend object for a Visualization.
     *
     * \return a pointer to the Visualization the backend provides.
     */
    PHONON_EXPORT QObject *createVisualization(QObject *parent = 0);
    /**
     * Create a new backend object for a VideoDataOutput.
     *
     * \return a pointer to the VideoDataOutput the backend provides.
     */
    PHONON_EXPORT QObject *createVideoDataOutput(QObject *parent = 0);
    /**
     * Create a new backend object for a VideoWidget.
     *
     * \return a pointer to the VideoWidget the backend provides.
     */
    QObject *createVideoWidget(QObject *parent = 0);

    /**
     * \return a pointer to the backend interface.
     */
    PHONON_EXPORT QObject *backend(bool createWhenNull = true);

    /**
     * Unique identifier for the Backend. Can be used in configuration files
     * for example.
     */
    QString identifier();

    /**
     * Get the name of the Backend. It's the name from the .desktop file.
     */
    PHONON_EXPORT QString backendName();

    /**
     * Get the comment of the Backend. It's the comment from the .desktop file.
     */
    QString backendComment();

    /**
     * Get the version of the Backend. It's the version from the .desktop file.
     *
     * The version is especially interesting if there are several versions
     * available for binary incompatible versions of the backend's media
     * framework.
     */
    QString backendVersion();

    /**
     * Get the icon (name) of the Backend. It's the icon from the .desktop file.
     */
    QString backendIcon();

    /**
     * Get the website of the Backend. It's the website from the .desktop file.
     */
    QString backendWebsite();

    /**
     * registers the backend object
     */
    PHONON_EXPORT QObject *registerQObject(QObject *o);

    bool isMimeTypeAvailable(const QString &mimeType);

    PHONON_EXPORT void registerFrontendObject(MediaNodePrivate *);
    PHONON_EXPORT void deregisterFrontendObject(MediaNodePrivate *);

    PHONON_EXPORT void setBackend(QObject *);
    //PHONON_EXPORT void createBackend(const QString &library, const QString &version = QString());

    PlatformPlugin *platformPlugin();

//X    It is probably better if we can get away with internal handling of
//X    freeing the soundcard device when it's not needed anymore and
//X    providing an IPC method to stop all MediaObjects -> free all
//X    devices
//X    /**
//X     * \internal
//X     * This is called when the application needs to free the soundcard
//X     * device(s).
//X     */
//X    void freeSoundcardDevices();
} // namespace Factory
} // namespace Phonon

QT_END_NAMESPACE
QT_END_HEADER

#endif // PHONON_FACTORY_P_H
// vim: sw=4 ts=4
