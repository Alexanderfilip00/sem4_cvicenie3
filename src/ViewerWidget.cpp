#include   "ViewerWidget.h"

ViewerWidget::ViewerWidget(QString viewerName, QSize imgSize, QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	setMouseTracking(true);
	name = viewerName;
	if (imgSize != QSize(0, 0)) {
		img = new QImage(imgSize, QImage::Format_ARGB32);
		img->fill(Qt::white);
		resizeWidget(img->size());
		setPainter();
		setDataPtr();
	}
}
ViewerWidget::~ViewerWidget()
{
	delete painter;
	delete img;
}
void ViewerWidget::resizeWidget(QSize size)
{
	this->resize(size);
	this->setMinimumSize(size);
	this->setMaximumSize(size);
}

//Image functions
bool ViewerWidget::setImage(const QImage& inputImg)
{
	if (img != nullptr) {
		delete img;
	}
	img = new QImage(inputImg);
	if (!img) {
		return false;
	}
	resizeWidget(img->size());
	setPainter();
	update();

	return true;
}
bool ViewerWidget::isEmpty()
{
	if (img->size() == QSize(0, 0)) {
		return true;
	}
	return false;
}

//Data function
void ViewerWidget::setPixel(int x, int y, const QColor& color)
{
	data[x + y * img->width()] = color.rgb();
}
void ViewerWidget::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	QColor color(r, g, b);
	setPixel(x, y, color);
}

void ViewerWidget::clear(QColor color)
{
	for (size_t x = 0; x < img->width(); x++)
	{
		for (size_t y = 0; y < img->height(); y++)
		{
			setPixel(x, y, color);
		}
	}
	update();
}

void ViewerWidget::KresliUseckuDDA(QPointF P1, QPointF P2, QColor* Color)
{
	P1.setX(round(P1.x()));
	P1.setY(round(P1.y()));
	P2.setX(round(P2.x()));
	P2.setY(round(P2.y()));

	int deltaX = P2.x() - P1.x();
	int deltaY = P2.y() - P1.y();
	float m = (float)deltaY / deltaX;

	if ((m >= 1.0) || (m <= -1.0)) {
		//riadiaca os Y
		if (P1.y() > P2.y()) {
			PointSwap(P1, P2);		//y1 ma byt mensie ako y2
		}
		float surX = P1.x();
		int surY = P1.y();
		do {
			setPixel(round(surX), surY, *Color);
			surY++;
			surX += 1 / m;
		} while (surY < P2.y());
	}
	else {
		//riadiaca os X
		if (P1.x() > P2.x()) {
			PointSwap(P1, P2);		//x1 ma byt mensie ako x2
		}
		int surX = P1.x();
		float surY = P1.y();
		do {
			setPixel(surX, round(surY), *Color);
			surX++;
			surY += m;
		} while (surX < P2.x());
	}
	update();
}
void ViewerWidget::KresliUseckuBresen(QPointF P1, QPointF P2, QColor* Color)
{
	P1.setX(round(P1.x()));
	P1.setY(round(P1.y()));
	P2.setX(round(P2.x()));
	P2.setY(round(P2.y()));

	int deltaX = P2.x() - P1.x();
	int deltaY = P2.y() - P1.y();
	float m = (float)deltaY / deltaX;
	int k1, k2, p, surX, surY;

	//riadiaca os Y

	if (m > 1.0) {

		if (P1.y() > P2.y()) {
			PointSwap(P1, P2);		//y1 ma byt mensie ako y2
			deltaX = P2.x() - P1.x();
			deltaY = P2.y() - P1.y();
		}

		k1 = 2 * deltaX;
		k2 = 2 * deltaX - 2 * deltaY;
		p = 2 * deltaX - deltaY;
		surX = P1.x();
		surY = P1.y();

		do {
			setPixel(surX, surY, *Color);
			surY++;
			if (p > 0) {
				surX++;
				p += k2;
			}
			else {
				p += k1;
			}
		} while (surY < P2.y());
	}

	else if (m < -1.0) {

		if (P1.y() > P2.y()) {
			PointSwap(P1, P2);		//y1 ma byt mensie ako y2
			deltaX = P2.x() - P1.x();
			deltaY = P2.y() - P1.y();
		}

		k1 = 2 * deltaX;
		k2 = 2 * deltaX + 2 * deltaY;
		p = 2 * deltaX + deltaY;
		surX = P1.x();
		surY = P1.y();

		do {
			setPixel(surX, surY, *Color);
			surY++;
			if (p < 0) {
				surX--;
				p += k2;
			}
			else {
				p += k1;
			}
		} while (surY < P2.y());
	}

	//riadiaca os X

	else if ((m >= 0.0) && (m <= 1.0)) {

		if (P1.x() > P2.x()) {
			PointSwap(P1, P2);		//x1 ma byt mensie ako x2
			deltaX = P2.x() - P1.x();
			deltaY = P2.y() - P1.y();
		}

		k1 = 2 * deltaY;
		k2 = 2 * deltaY - 2 * deltaX;
		p = 2 * deltaY - deltaX;
		surX = P1.x();
		surY = P1.y();

		do {
			setPixel(surX, surY, *Color);
			surX++;
			if (p > 0) {
				surY++;
				p += k2;
			}
			else {
				p += k1;
			}
		} while (surX < P2.x());
	}
	else if ((m >= -1.0) && (m < 0.0)) {

		if (P1.x() > P2.x()) {
			PointSwap(P1, P2);		//x1 ma byt mensie ako x2
			deltaX = P2.x() - P1.x();
			deltaY = P2.y() - P1.y();

		}

		k1 = 2 * deltaY;
		k2 = 2 * deltaY + 2 * deltaX;
		p = 2 * deltaY + deltaX;
		surX = P1.x();
		surY = P1.y();

		do {
			setPixel(surX, surY, *Color);
			surX++;
			if (p < 0) {
				surY--;
				p += k2;
			}
			else {
				p += k1;
			}
		} while (surX < P2.x());
	}
	update();
}

void ViewerWidget::KresliPolygonDDA(QVector<QPointF>* Sur, QColor* Color)
{
	int i;
	if (Sur->size() != 2) {
		for (i = 0; i < Sur->size() - 1; i++) {
			KresliUseckuDDA(Sur->at(i), Sur->at(i + 1), Color);
		}
	}
	if (Sur->size() >= 2) {
		KresliUseckuDDA(Sur->at(0), Sur->at(Sur->size() - 1), Color);
	}
}
void ViewerWidget::KresliPolygonBresen(QVector<QPointF>* Sur, QColor* Color)
{
	int i;
	if (Sur->size() != 2) {
		for (i = 0; i < Sur->size() - 1; i++) {
			KresliUseckuBresen(Sur->at(i), Sur->at(i + 1), Color);
		}
	}
	if (Sur->size() >= 2) {
		KresliUseckuBresen(Sur->at(0), Sur->at(Sur->size() - 1), Color);
	}
}

void ViewerWidget::Orezanie(QVector<QPointF>* Sur, QColor* Color, int Algoritmus)
{
	if (Sur->size() == 2) {
		//USECKA
		QVector<QPointF> V = *Sur;  //dolezite: vytvorim kopiu povodnych suradnic a narabam len s nou
		OrezanieUsecka(V);

		if(V.size()==2){			//touto podmienkou ziskam moznost, aby som niekedy ani nevykresloval usecku
			if (Algoritmus == 0) {
				KresliUseckuDDA(V[0], V[1], Color);
			}
			else {
				KresliUseckuBresen(V[0], V[1], Color);
			}
		}
	}
	else {
		//POLYGON
		QVector<QPointF> V = *Sur;		//dolezite: vytvorim kopiu povodnych suradnic a narabam len s nou
		OrezaniePolygon(&V);

		if (Algoritmus == 0) {
			KresliPolygonDDA(&V, Color);
		}
		else {
			KresliPolygonBresen(&V, Color);
		}
	}
}

void ViewerWidget::OrezaniePolygon(QVector<QPointF>* V) {

	QVector<QPointF>W;		//vektor bodov orezaneho polygonu
	QPointF S, Pi;
	int i, j;
	float Xmin[4] = { 0.0,0.0, -(float)getImgWidth() + 1, -(float)getImgHeight() + 1 };

	for (j = 0; j < 4; j++) {

		if (V->size() != 0) {
			S = V->at(V->size() - 1);
		}

		for (i = 0; i < V->size(); i++) {

			if (V->at(i).x() >= Xmin[j]) {
				if (S.x() >= Xmin[j]) {
					W.push_back(V->at(i));
				}
				else {
					Pi = QPoint(Xmin[j], S.y() + ((Xmin[j] - S.x()) * (V->at(i).y() - S.y()) / (V->at(i).x() - S.x())));
					W.push_back(Pi);
					W.push_back(V->at(i));
				}
			}
			else {
				if (S.x() >= Xmin[j]) {
					Pi = QPoint(Xmin[j], S.y() + ((Xmin[j] - S.x()) * (V->at(i).y() - S.y()) / (V->at(i).x() - S.x())));
					W.push_back(Pi);
				}
			}
			S = V->at(i);
		}

		V->clear();
		V->squeeze();
		for (i = 0; i < W.size(); i++) {
			V->push_back(QPointF(W[i].y(), -W[i].x()));
		}
		W.clear();
		W.squeeze();
	}
}

void ViewerWidget::OrezanieUsecka(QVector<QPointF>& V)
{
	QPointF d = V[1] - V[0];		//smernica usecky

	QVector<QPointF> E(4);
	E[0] = QPointF(0.0, 0.0);
	E[1] = QPointF(0.0, getImgHeight());
	E[2] = QPointF(getImgWidth(), getImgHeight());
	E[3] = QPointF(getImgWidth(), 0.0);

	//##############################################

	if (KontrolaPriesecnikov(V, E, d)) {
		//skontrolovalo, ci usecka presekla platno
		float tl = 0.0;
		float tu = 1.0;
		QPointF P0 = V[0];
		QPointF P1 = V[1];

		for (int i = 0; i < 4; i++) {
			QPointF smernicaE = E[(i + 1) % 4] - E[i];
				QPointF n(smernicaE.y(), -smernicaE.x());		//vnutorna normala
				QPointF w = V[0] - E[i];

				float dn = SkalarSuc(&d, &n);
				float wn = SkalarSuc(&w, &n);

				if (dn != 0.0) {
					float t = -wn / dn;
						if (dn > 0.0 && t <= 1.0) {
							tl = max(t, tl);
						}
						else if (dn < 0.0 && t >= 0.0) {
							tu = min(t, tu);
						}
				}

		}

		if (tl < tu) {
			V[0] = P0 + tl * (P1 - P0);
			V[1] = P0 + tu * (P1 - P0);
		}
		else {
			V.clear();
			V.squeeze();
		}
	}
}


void ViewerWidget::PointSwap(QPointF& P1, QPointF& P2)
{
	QPointF P3 = P1;
	P1 = P2;
	P2 = P3;
}
float ViewerWidget::SkalarSuc(QPointF* P1, QPointF* P2)
{
	return (P1->dotProduct(*P1, *P2));
}
float ViewerWidget::SkalarSuc(QPoint* P1, QPoint* P2)
{
	return (P1->dotProduct(*P1, *P2));
}
bool ViewerWidget::KontrolaPriesecnikov(QVector<QPointF>& V, QVector<QPointF>& E, QPointF& d)
{
	int i;
	if (!((V[0].x() >= 1.0 && V[0].x() <= getImgWidth() &&		//kontrola, ci su dva body mimo platna
		V[0].y() >= 1.0 && V[0].y() <= getImgHeight()) ||
		(V[1].x() >= 1.0 && V[1].x() <= getImgWidth() &&
			V[1].y() >= 1.0 && V[1].y() <= getImgHeight())))
	{
		//ak su oba body mimo platna, otestuje, kolko priesecnikov existuje medzi nasou useckou a hranami platna
		//ak to nie su dva priesecniky, nevypocitaju sa ziadne body a funkcia sa ukonci
		//algoritmus som si vymyslel sam, rovnice pre priesecnik dvoch useciek som nasiel na internete
		QVector<QPointF> smerniceE(4);
		int pretnutia = 0;
		for (i = 0; i < 4; i++) {
			smerniceE[i] = E[(i + 1) % 4] - E[i];
			float s, t;
			s = (-d.y() * (V[0].x() - E[i].x()) + d.x() * (V[0].y() - E[i].y())) / (-smerniceE[i].x() * d.y() + d.x() * smerniceE[i].y());
			t = (smerniceE[i].x() * (V[0].y() - E[i].y()) - smerniceE[i].y() * (V[0].x() - E[i].x())) / (-smerniceE[i].x() * d.y() + d.x() * smerniceE[i].y());
			if (s > 0 && s < 1 && t > 0 && t < 1) {
				pretnutia++;
			}
		}
		if (pretnutia != 2) {
			V.clear();
			V.squeeze();
			return false;
		}
	}

	//vrati true bud ak su oba body na platne alebo ak existuju dva priesecniky platna useckou
	return true;	
}

//Slots
void ViewerWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QRect area = event->rect();
	painter.drawImage(area, *img, area);
}
