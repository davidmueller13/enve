#ifndef BOXESGROUP_H
#define BOXESGROUP_H
#include "boundingbox.h"
#include "vectorpath.h"
#include "fillstrokesettings.h"

#define getAtIndexOrGiveNull(index, list) (( (index) >= (list).count() || (index) < 0 ) ? NULL : (list).at( (index) ))

#define foreachBoxInListInverted(boxesList) BoundingBox *box = getAtIndexOrGiveNull((boxesList).count() - 1, (boxesList)); \
    for(int i = (boxesList).count() - 1; i >= 0; i--, box = getAtIndexOrGiveNull(i, (boxesList)) )

class MainWindow;

class BoxesGroup : public BoundingBox
{
public:
    BoxesGroup(FillStrokeSettingsWidget *fillStrokeSetting, BoundingBox *parent);
    BoxesGroup(FillStrokeSettingsWidget *fillStrokeSetting, MainWindow *parent);
    BoxesGroup(int boundingBoxId,
               FillStrokeSettingsWidget *fillStrokeSetting, BoundingBox *parent);

    bool pointInsidePath(QPointF absPos);
    QRectF getBoundingRect();
    void draw(QPainter *p);
    void drawSelected(QPainter *p, CanvasMode currentCanvasMode);
    void removeSelectedPointsAndClearList();
    void removeSelectedBoxesAndClearList();
    void removeChildFromList(int id, bool saveUndoRedo);
    void clearBoxesSelection();
    void removePointFromSelection(MovablePoint *point);
    void removeBoxFromSelection(BoundingBox *box);
    void addPointToSelection(MovablePoint *point);
    void addBoxToSelection(BoundingBox *box);
    void clearPointsSelection();
    void bringSelectedBoxesToFront();
    void bringSelectedBoxesToEnd();
    void moveSelectedBoxesUp();
    void moveSelectedBoxesDown();
    void selectAllBoxes();
    void deselectAllBoxes();
    void setPointCtrlsMode(CtrlsMode mode);
    void mergePoints();
    void disconnectPoints();
    void connectPoints();
    BoundingBox *getBoxAt(QPointF absPos);
    MovablePoint *getPointAt(QPointF absPos, CanvasMode currentMode);
    void selectAndAddContainedPointsToSelection(QRectF absRect);
    void finishSelectedPointsTransform();
    void addContainedBoxesToSelection(QRectF rect);
    void finishSelectedBoxesTransform();
    void moveSelectedPointsBy(QPointF by, bool startTransform);
    void moveSelectedBoxesBy(QPointF by, bool startTransform);
    bool isShiftPressed();
    bool isCtrlPressed();
    BoxesGroup *groupSelectedBoxes();
    void drawBoundingRect(QPainter *p);

    void setIsCurrentGroup(bool bT);

    BoundingBox *getBoxAtFromAllAncestors(QPointF absPos);

    void setFillSettings(PaintSettings fillSettings,
                         bool saveUndoRedo = true);
    void setStrokeSettings(StrokeSettings strokeSettings,
                           bool saveUndoRedo = true);
    void setSelectedFillSettings(PaintSettings fillSettings, bool saveUndoRedo = true);
    void setSelectedStrokeSettings(StrokeSettings strokeSettings, bool saveUndoRedo = true);
    void setCurrentFillStrokeSettingsFromBox(BoundingBox *box);

    void startStrokeTransform();
    void startFillTransform();
    void finishStrokeTransform();
    void finishFillTransform();

    void startSelectedStrokeTransform();
    void startSelectedFillTransform();
    void finishSelectedStrokeTransform();
    void finishSelectedFillTransform();

    void rotateSelectedBy(qreal rotBy, QPointF absOrigin, bool startTrans);

    QPointF getSelectedPivotPos();
    bool isSelectionEmpty();
    void setSelectedPivotAbsPos(QPointF absPos);

    void ungroupSelected();
    void ungroup();
    void updatePivotPosition();

    void select();
    void scaleSelectedBy(qreal scaleBy, QPointF absOrigin, bool startTrans);
    void cancelSelectedBoxesTransform();

    int saveToSql(int parentId);
    void loadChildrenFromSql(QString thisBoundingBoxId);
    PathPoint *createNewPointOnLineNearSelected(QPointF absPos);
    void saveSelectedToSql();
protected:
    FillStrokeSettingsWidget *mFillStrokeSettingsWidget;
    bool mIsCurrentGroup = false;
    QList<MovablePoint*> mSelectedPoints;
    QList<BoundingBox*> mSelectedBoxes;
};

#endif // BOXESGROUP_H
