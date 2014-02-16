/********************************************************************************
 * Copyright (C) 2011 by Stephen Allewell                                       *
 * sallewell@users.sourceforge.net                                              *
 *                                                                              *
 * This program is free software; you can redistribute it and/or modify         *
 * it under the terms of the GNU General Public License as published by         *
 * the Free Software Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                          *
 ********************************************************************************/


/**
 * @file
 * Header file for the Editor class.
 */


#ifndef Editor_H
#define Editor_H


#include <QMap>
#include <QPainterPath>
#include <QPair>
#include <QPointF>
#include <QUndoStack>
#include <QWidget>

#include "Commands.h"


class QMousePressEvent;
class QMouseMoveEvent;
class QMouseReleaseEvent;
class QPaintEvent;


/**
 * @brief Manages the editor window allowing user interaction with the various tools.
 *
 * The Editor class is used to edit the new symbols or the symbols from the loaded library.
 * It shows a grid of @ref gridElements of @ref elementSize grouped into @ref elementGroup
 * number of elements.
 *
 * The path is a QPainterPath element and is constructed from the various elements created
 * by the tools. The constructed QPainterPath is used to draw the path in the editor so it
 * is an accurate representation of the final path. The QPainterPath is deconstructed into
 * its individual elements and points for editing.
 *
 * There are additional attributes used to draw the path, filled or unfilled, the fill rule,
 * line width, end cap and join styles can be changed to alter the appearance of the path.
 *
 * The user interacts with the editor using the various tools. For each tool a number of
 * points need to be defined, simple tools like move to and line to only require one point
 * whereas cubic to requires three to define the two control points and the end point.
 *
 * Convenience tools are available for rectangles and ellipses but these are broken down
 * into lines and curves in the constructed path.
 *
 * Points that have been added can be moved to adjust the shape by dragging them around.
 * The points will snap to the grid intersections if the snap option is enabled. Otherwise
 * they can be positioned anywhere.
 *
 * Tools are available to rotate or flip the symbol which adjusts the points positions and
 * reconstructs the QPainterPath.
 *
 * Guide lines and circles will be displayed if the current position of the mouse on the
 * editor aligns with an existing point either horizontally, vertically or at a specified
 * angle, or if the point lies on the same circle as the original where its origin is at
 * the center of the grid.
 */
class Editor : public QWidget
{
    Q_OBJECT

public:
    enum ToolMode {MoveTo, LineTo, CubicTo, Rectangle, Ellipse};

    explicit Editor(QWidget *parent = 0);
    ~Editor();

    QPair<qint16, Symbol> symbol();
    void setSymbol(const QPair<qint16, Symbol> &pair);

    QPainterPath moveTo(const QPointF &to);
    QPainterPath lineTo(const QPointF &to);
    QPainterPath cubicTo(const QPointF &control1, const QPointF &control2, const QPointF &to);
    QPainterPath addRectangle(const QPointF &from, const QPointF &to);
    QPainterPath addEllipse(const QPointF &from, const QPointF &to);
    void removeLast(const QPainterPath &path);
    void movePoint(int index, const QPointF &to);
    void rotatePointsLeft();
    void rotatePointsRight();
    void flipPointsHorizontal();
    void flipPointsVertical();
    void setFilled(bool filled);
    void setFillRule(Qt::FillRule rule);
    void setCapStyle(Qt::PenCapStyle capStyle);
    void setJoinStyle(Qt::PenJoinStyle joinStyle);
    void setLineWidth(double width);

    void clear();

    QUndoStack *undoStack();

public slots:
    void selectTool(QAction *action);
    void enableSnap(bool enabled);
    void selectFilled(bool enabled);
    void selectFillRule(QAction *action);
    void selectCapStyle(QAction *action);
    void selectJoinStyle(QAction *action);
    void increaseLineWidth();
    void decreaseLineWidth();

    void rotateLeft();
    void rotateRight();
    void flipHorizontal();
    void flipVertical();

signals:
    void message(const QString &text);
    void minLineWidth(bool reached);
    void maxLineWidth(bool reached);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);

    void keyPressEvent(QKeyEvent *event);

private:
    void addPoint(const QPointF &point);
    QPointF snapPoint(const QPoint &point) const;
    QPair<bool, QPointF> snapToGrid(const QPoint &point) const;
    QPair<bool, QPointF> snapToGuide(const QPointF &point) const;
    QPointF toSymbol(const QPoint &point) const;
    QPoint toScreen(const QPointF &point) const;
    bool node(const QPointF &point) const;
    QPair<bool, int> nodeUnderCursor(const QPointF &point) const;
    void deconstructPainterPath();
    void constructPainterPath();
    bool constructGuides(const QPointF &to);
    void constructLineGuides(const QPointF &from, const QPointF &to);
    void constructCircleGuides(const QPointF &from, const QPointF &to);
    QLineF projected(const QLineF &line) const;

    int     m_size;                                 /**< the overall size of the editor */

    bool    m_snap;                                 /**< true if snap mode is enabled */
    bool    m_fill;                                 /**< true if fill mode is enabled */

    QUndoStack  m_undoStack;                        /**< holds the commands that modify the editor contents allowing for undo */

    ToolMode    m_toolMode;                         /**< the current tool selected */

    QList<QPointF>                      m_points;   /**< the points that have been committed relating to the elements in m_elements */
    QList<QPainterPath::ElementType>    m_elements; /**< the basic elements making up the path, move, line and curve */

    QList<QPointF>      m_activePoints;             /**< the points that are being entered for the current command */

    qint16              m_index;                    /**< the index of the symbol as stored in the library, this is 0 for new symbols */
    QPainterPath        m_painterPath;              /**< the path from m_symbol currently being edited */
    Symbol              m_symbol;                   /**< the symbol containing the QPainterPath and rendering attributes */

    bool                m_dragging;                 /**< true if currently dragging a point around */
    QPointF             m_start;                    /**< the start position of a drag operation or the start of a rubber band selection */
    QPointF             m_tracking;                 /**< the current position of a drag operation or the position of a rubber band selection */
    QRectF              m_rubberBand;               /**< a rubber band rectangle in symbol coordinates, is null when not required */
    QPair<bool, int>    m_dragPointIndex;           /**< represents the list and index of the point being moved, true for m_points, false for m_activePoints */

    QVector<qreal>      m_angles;                   /**< the angles allowed for constructing guide lines */
    QList<QLineF>       m_guideLines;               /**< the guide lines that have been constructed for a given point */
    QList<qreal>        m_guideCircles;             /**< the guide circles that have been constructed for a given point */
    QList<QPointF>      m_snapPoints;               /**< points that intersect with guide lines */
    QLineF              m_topEdge;                  /**< represents the top edge of the editor from 0,0 to 1,0 */
    QLineF              m_bottomEdge;               /**< represents the bottom edge of the editor from 0,1 to 1,1 */
    QLineF              m_leftEdge;                 /**< represents the left edge of the editor from 0,1 to 0,1 */
    QLineF              m_rightEdge;                /**< represents the right edge of the editor from 1,0 to 1,1 */
};


#endif

