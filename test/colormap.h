#ifndef __COLORMAP_H__
#define __COLORMAP_H__
#include <QVector>
#include <QRgb>
enum  COLORMAP
{
    COLORMAP_AUTUMN,
    COLORMAP_BONE,
    COLORMAP_COOL,
    COLORMAP_HOT,
    COLORMAP_HSV,
    COLORMAP_JET,
    COLORMAP_OCEAN,
    COLORMAP_PARULA,
    COLORMAP_PINK,
    COLORMAP_RAINBOW,
    COLORMAP_SPRING,
    COLORMAP_SUMMER,
    COLORMAP_TURBO,
    COLORMAP_WINTER
};

QVector<QRgb> Autumn();
QVector<QRgb> Bone();
QVector<QRgb> Cool();
QVector<QRgb> Hot();
QVector<QRgb> Hsv();
QVector<QRgb> Jet();
QVector<QRgb> Ocean();
QVector<QRgb> Parula();
QVector<QRgb> Pink();
QVector<QRgb> Rainbow();
QVector<QRgb> Spring();
QVector<QRgb> Summer();
QVector<QRgb> Turbo();
QVector<QRgb> Winter();

#endif//__COLORMAP_H__
