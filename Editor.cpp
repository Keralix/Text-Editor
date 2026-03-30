#include "Editor.h"
#include <QPainter>
#include <iostream>

Editor::Editor(QWidget *parent) : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    font = QFont("Arial", 16);
    textColor = Qt::black;
    cursorPosX = 0;
    cursorPosY = 0;
    preferredPos = 0;
    cursorVisible = true;
    anchorPosX = 0;
    anchorPosY = 0;
    hasSelection = false;
    selecting = false;
    selectStartX = 0;
    selectStartY = 0;
    selectEndX = 0;
    selectEndY = 0;

    cursorTimer = new QTimer(this);
    connect(cursorTimer, &QTimer::timeout, this, [this](){
        cursorVisible = !cursorVisible;
        update();
    });
    cursorTimer->start(500);
}

QStringList Editor::getLines() {
    return QString::fromStdString(text.getText()).split('\n');
}

int Editor::toIndex(int line, int col) {
    QStringList lines = getLines();
    int index = 0;
    for (int i = 0; i < line; i++) {
        index += lines[i].length() + 1; // +1 za '\n'
    }
    index += col;
    return index;
}

void Editor::updateSelection() {
    selectStartX = anchorPosX;
    selectStartY = anchorPosY;
    selectEndX = cursorPosX;
    selectEndY = cursorPosY;
    hasSelection = true;
}

void Editor::clearSelection() {
    anchorPosX = cursorPosX;
    anchorPosY = cursorPosY;
    selectStartX = cursorPosX;
    selectStartY = cursorPosY;
    selectEndX = cursorPosX;
    selectEndY = cursorPosY;
    hasSelection = false;
}

void Editor::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);
    painter.setFont(font);

    QFontMetrics fm(font);
    int x = 5;
    int lineHeight = fm.height();

    QStringList lines = QString::fromStdString(text.getText()).split('\n');

    // crtanje selekcije - PRIJE teksta
    if (hasSelection || selectStartX != selectEndX || selectStartY != selectEndY) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 120, 215, 100));

        int startY, startX, endY, endX;
        if (selectStartY < selectEndY ||
           (selectStartY == selectEndY && selectStartX < selectEndX)) {
            startY = selectStartY; startX = selectStartX;
            endY   = selectEndY;   endX   = selectEndX;
        } else {
            startY = selectEndY;   startX = selectEndX;
            endY   = selectStartY; endX   = selectStartX;
        }

        for (int i = startY; i <= endY; i++) {
            int sCol = (i == startY) ? startX : 0;
            int eCol = (i == endY)   ? endX   : lines[i].length();

            int sX = x + fm.horizontalAdvance(lines[i].left(sCol));
            int eX = x + fm.horizontalAdvance(lines[i].left(eCol));
            int y  = 5 + i * lineHeight;
            painter.drawRect(sX, y, eX - sX, lineHeight);
        }
    }

    // crtanje teksta
    painter.setPen(textColor);
    for (int i = 0; i < lines.size(); i++) {
        int y = 5 + fm.ascent() + i * lineHeight;
        painter.drawText(x, y, lines[i]);
    }

    // crtanje kursora
    if (cursorVisible) {
        int cursorX = x + fm.horizontalAdvance(lines[cursorPosY].left(cursorPosX));
        int cursorY = 5 + cursorPosY * lineHeight;
        painter.setPen(Qt::black);
        painter.drawLine(cursorX, cursorY, cursorX, cursorY + lineHeight);
    }
}

void Editor::keyPressEvent(QKeyEvent *event)
{
    QStringList lines = getLines();

    if (event->modifiers() & Qt::ControlModifier) {
        if (event->key() == Qt::Key_B) {
            font.setBold(!font.bold());
        } else if (event->key() == Qt::Key_I) {
            font.setItalic(!font.italic());
        } else if (event->key() == Qt::Key_Plus) {
            font.setPointSize(font.pointSize() + 2);
        } else if (event->key() == Qt::Key_Minus) {
            font.setPointSize(std::max(6, font.pointSize() - 2));
        }
    }

    else if (event->modifiers() & Qt::ShiftModifier) {
        if (!hasSelection) {
            anchorPosX = cursorPosX;
            anchorPosY = cursorPosY;
        }

        if (event->key() == Qt::Key_Left) {
            if (cursorPosX > 0) {
                cursorPosX--;
            } else if (cursorPosY > 0) {
                cursorPosY--;
                cursorPosX = getLines()[cursorPosY].length();
            }
            preferredPos = cursorPosX;
            updateSelection();
        }
        else if (event->key() == Qt::Key_Right) {
            if (cursorPosX < lines[cursorPosY].length()) {
                cursorPosX++;
            } else if (cursorPosY < lines.length() - 1) {
                cursorPosY++;
                cursorPosX = 0;
            }
            preferredPos = cursorPosX;
            updateSelection();
        }
        else if (event->key() == Qt::Key_Up) {
            if (cursorPosY > 0) {
                cursorPosY--;
                cursorPosX = std::min(preferredPos, (int)getLines()[cursorPosY].length());
            }
            updateSelection();
        }
        else if (event->key() == Qt::Key_Down) {
            if (cursorPosY < lines.length() - 1) {
                cursorPosY++;
                cursorPosX = std::min(preferredPos, (int)getLines()[cursorPosY].length());
            }
            updateSelection();
        }
    }

    else if (event->key() == Qt::Key_Backspace) {
        if (cursorPosX > 0) {
            text.deleteAt(toIndex(cursorPosY, cursorPosX) - 1);
            cursorPosX--;
        } else if (cursorPosY > 0) {
            text.deleteAt(toIndex(cursorPosY, cursorPosX) - 1);
            cursorPosY--;
            cursorPosX = getLines()[cursorPosY].length();
        }
        clearSelection();
        preferredPos = cursorPosX;
    }
    else if (event->key() == Qt::Key_Delete) {
        if (cursorPosX < lines[cursorPosY].length()) {
            text.deleteAt(toIndex(cursorPosY, cursorPosX));
        } else if (cursorPosY < lines.size() - 1) {
            text.deleteAt(toIndex(cursorPosY, cursorPosX));
        }
        clearSelection();
    }
    else if (event->key() == Qt::Key_Left) {
        if (cursorPosX > 0) {
            cursorPosX--;
        } else if (cursorPosY > 0) {
            cursorPosY--;
            cursorPosX = lines[cursorPosY].length();
        }
        clearSelection();
        preferredPos = cursorPosX;
    }
    else if (event->key() == Qt::Key_Right) {
        if (cursorPosX < lines[cursorPosY].length()) {
            cursorPosX++;
        } else if (cursorPosY < lines.length() - 1) {
            cursorPosX = 0;
            cursorPosY++;
        }
        clearSelection();
        preferredPos = cursorPosX;
    }
    else if (event->key() == Qt::Key_Up) {
        if (cursorPosY > 0) {
            cursorPosY--;
            cursorPosX = std::min(preferredPos, (int)getLines()[cursorPosY].length());
        }
        clearSelection();
    }
    else if (event->key() == Qt::Key_Down) {
        if (cursorPosY < lines.length() - 1) {
            cursorPosY++;
            cursorPosX = std::min(preferredPos, (int)getLines()[cursorPosY].length());
        }
        clearSelection();
    }
    else if (event->key() == Qt::Key_Return) {
        text.insertAt(toIndex(cursorPosY, cursorPosX), '\n');
        cursorPosY++;
        cursorPosX = 0;
        clearSelection();
        preferredPos = 0;
    }
    else if (event->key() == Qt::Key_Escape) {
        window()->close();
    }
    else if (!event->text().isEmpty()) {
        text.insertAt(toIndex(cursorPosY, cursorPosX), event->text().toStdString()[0]);
        cursorPosX++;
        clearSelection();
        preferredPos = cursorPosX;
    }

    cursorVisible = true;
    update();
}

void Editor::calculateMousePosition(int mousePosX, int mousePosY) {
    QFontMetrics fm(font);
    int x = 5;
    QStringList lines = getLines();
    int clickY = mousePosY - 5;
    int line = clickY / fm.height();
    line = std::max(0, std::min(line, (int)lines.size() - 1));
    QString lineText = lines[line];
    int col = 0;
    int bestDist = std::abs(mousePosX - x);
    for (int i = 1; i <= lineText.length(); i++) {
        int charX = x + fm.horizontalAdvance(lineText.left(i));
        int dist = std::abs(mousePosX - charX);
        if (dist < bestDist) {
            bestDist = dist;
            col = i;
        } else {
            break;
        }
    }
    cursorPosY = line;
    cursorPosX = col;
    preferredPos = col;
}

void Editor::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        calculateMousePosition(event->pos().x(), event->pos().y());
        cursorVisible = true;
        selecting = true;
        clearSelection();
        update();
    }
}

void Editor::mouseMoveEvent(QMouseEvent *event) {
    this->setCursor(QCursor(Qt::IBeamCursor));
    if (!selecting) return;
    calculateMousePosition(event->pos().x(), event->pos().y());
    updateSelection();
    update();
}

void Editor::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    selecting = false;
}