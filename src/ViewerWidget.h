#pragma once
#include <QtWidgets>
class ViewerWidget :public QWidget {
	Q_OBJECT
private:
	QString name = "";
	QSize areaSize = QSize(0, 0);
	QImage* img = nullptr;
	QRgb* data = nullptr;
	QPainter* painter = nullptr;


public:
	ViewerWidget(QString viewerName, QSize imgSize, QWidget* parent = Q_NULLPTR);
	~ViewerWidget();
	void resizeWidget(QSize size);

	//Image functions
	bool setImage(const QImage& inputImg);
	QImage* getImage() { return img; };
	bool isEmpty();

	//Data functions
	QRgb* getData() { return data; }
	void setPixel(int x, int y, const QColor& color);
	void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
	//bool isInside(int x, int y) { return (x >= 0 && y >= 0 && x < img->width() && y < img->height()) ? true : false; }

	//Get/Set functions
	QString getName() { return name; }
	void setName(QString newName) { name = newName; }

	void setPainter() { painter = new QPainter(img); }
	void setDataPtr() { data = reinterpret_cast<QRgb*>(img->bits()); }

	int getImgWidth() { return img->width(); };
	int getImgHeight() { return img->height(); };

	void clear(QColor color = Qt::white);

	void KresliUseckuDDA(QPointF P1, QPointF P2, QColor* Color);
	void KresliUseckuBresen(QPointF P1, QPointF P2, QColor* Color);

	void KresliPolygonDDA(QVector<QPointF>* Sur, QColor* Color);
	void KresliPolygonBresen(QVector<QPointF>* Sur, QColor* Color);

	void Orezanie(QVector<QPointF>* Sur, QColor* Color, int Algoritmus);
	void OrezaniePolygon(QVector<QPointF>* V);
	void OrezanieUsecka(QVector<QPointF>& V);


	//POMOCNE FUNKCIE
	void PointSwap(QPointF& P1, QPointF& P2);
	float SkalarSuc(QPointF* P1, QPointF* P2);
	float SkalarSuc(QPoint* P1, QPoint* P2);
	float max(float c1, float c2) { return (c1 >= c2) ? c1 : c2; };
	float min(float c1, float c2) { return (c1 <= c2) ? c1 : c2; };
	bool KontrolaPriesecnikov(QVector<QPointF>& V, QVector<QPointF>& E, QPointF& d);



public slots:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
};