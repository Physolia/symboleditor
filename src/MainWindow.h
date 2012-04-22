/********************************************************************************
 * Copyright (C) 2011 by Stephen Allewell                                       *
 * stephen@mirramar.adsl24.co.uk                                                *
 *                                                                              *
 * This program is free software; you can redistribute it and/or modify         *
 * it under the terms of the GNU General Public License as published by         *
 * the Free Software Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                          *
 ********************************************************************************/


/**
 * @file
 * Header file for the MainWindow class.
 */


#ifndef MainWindow_H
#define MainWindow_H


#include <QUndoGroup>

#include <KXmlGuiWindow>


class QListWidget;
class QListWidgetItem;
class KTabWidget;

class KUrl;
class Editor;
class SymbolLibrary;


/**
 * @brief Manages the main window of the application.
 *
 * The MainWindow class is based on the KXmlGuiWindow class which provides the basis for KDE applications.
 * It creates instances of the Editor class and the QListWidget class that is used to show the existing
 * symbols in the library.
 *
 * It creates all the actions that are associated with the application connecting various signals to the
 * relevant slots to allow the interaction between the gui elements.
 */
class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected:
    virtual bool queryClose();
    virtual bool queryExit();

protected slots:
    // File menu
    void fileOpen();
    void fileOpen(const KUrl &url);
    void save();
    void saveAs();
    void newSymbol();
    void saveSymbol();
    void saveSymbolAsNew();
    void importLibrary();
    void close();
    void quit();

    // Edit menu
    void undo();
    void redo();
    void undoTextChanged(const QString &text);
    void redoTextChanged(const QString &text);
    void cleanChanged(bool clean);

    void currentChanged(int index);
    void itemDoubleClicked(QListWidgetItem *item);

private:
    bool editorClean();
    bool libraryClean();
    void setupActions();

    KTabWidget  *m_tabWidget;           /**< pointer to the KTabWidget containing the editor and library tabs */
    Editor      *m_editor;              /**< pointer to the Editor */
    QListWidget *m_listWidget;          /**< pointer to the QListWidget containing icons for the library symbols */

    SymbolLibrary   *m_symbolLibrary;   /**< pointer to a SymbolLibrary */

    QUndoGroup  m_undoGroup;            /**< the QUndoGroup has the QUndoStacks for the Editor and the SymbolLibrary added to it */
};


#endif

