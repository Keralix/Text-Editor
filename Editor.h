#pragma once
#include <QtWidgets>
#include <QTimer>
#include "Text.h"

class Editor : public QWidget {
    Q_OBJECT
public:
    Editor(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

private:
    // tekst
    Text text;

    // font i boja
    QFont font;
    QColor textColor;

    int cursorPosX, cursorPosY;
    int preferredPos;
    bool cursorVisible;
    QTimer *cursorTimer;

    // selekcija - anchor je gdje je selekcija pocela
    int anchorPosX, anchorPosY;
    bool hasSelection;
    int selectStartX, selectStartY;
    int selectEndX, selectEndY;
    bool selecting; // true dok drzis mis pritisnut

    // helper funkcije
    QStringList getLines();
    int toIndex(int line, int col);
    void calculateMousePosition(int mousePosX, int mousePosY);
    void updateSelection();
    void clearSelection();
};