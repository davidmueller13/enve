#ifndef RENDERCACHEHANDLER_H
#define RENDERCACHEHANDLER_H
#include <QtCore>
#include <memory>
#include "sharedpointerdefs.h"
class QPainter;
class CacheContainer;
class RenderCacheHandler;

class CacheHandler {
public:
    virtual ~CacheHandler() {}
    void removeRenderContainer(const CacheContainerSPtr &cont);
    CacheContainer *getRenderContainerAtRelFrame(const int &frame);
    virtual CacheContainer *createNewRenderContainerAtRelFrame(const int &frame);
    int getFirstEmptyFrameAtOrAfterFrame(const int &frame);
    void setContainersInFrameRangeBlocked(const int &minFrame,
                                          const int &maxFrame,
                                          const bool &blocked);

    void drawCacheOnTimeline(QPainter *p,
                             const qreal &pixelsPerFrame,
                             const qreal &drawY,
                             const int &startFrame,
                             const int &endFrame);
    void clearCache();
    CacheContainer *getRenderContainerAtOrBeforeRelFrame(const int &frame);
    int getFirstEmptyFrameAfterFrame(const int &frame);
    int getMaxPreviewFrame(const int &minFrame,
                           const int &maxFrame,
                           const int &frameSize);
    void cacheDataBeforeRelFrame(const int &relFrame);
    void cacheDataAfterRelFrame(const int &relFrame);
    CacheContainer *getRenderContainerAtOrAfterRelFrame(const int &frame);
    int getContainerCountAfterRelFrame(const int &relFrame);
    void cacheLastContainer();
    int getRenderContainerIdAtOrAfterRelFrame(const int &frame);
    int getRenderContainerIdAtOrBeforeRelFrame(const int &frame);
    void cacheFirstContainer();
    int getNumberNotCachedBeforeRelFrame(const int &relFrame);
    void updateAllAfterFrameInMemoryHandler(const int &relFrame);
    int getFirstEmptyOrCachedFrameAfterFrame(const int &frame,
                                             CacheContainer **contP = nullptr);
    void clearCacheForRelFrameRange(const int &minFrame,
                                    const int &maxFrame);
protected:
    int getRenderContainterInsertIdAtRelFrame(const int &relFrame);
    bool getRenderContainterIdAtRelFrame(const int &relFrame, int *id);
    QList<CacheContainerSPtr> mRenderContainers;
};

#endif // RENDERCACHEHANDLER_H