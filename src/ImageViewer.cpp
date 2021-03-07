#include "ImageViewer.h"

ImageViewer::ImageViewer(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::ImageViewerClass)
{
	ui->setupUi(this);

	//zapne okno pri starte
	openNewTabForImg(new ViewerWidget("default canvas", QSize(800, 800)));
	ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);

	ui->ButtonShear->setEnabled(true);
	ui->ButtonRot->setEnabled(true);
	ui->ButtonScale->setEnabled(true);
	ui->ButtonSymm->setEnabled(true);
	ui->SpinShear->setEnabled(true);
}

//ViewerWidget functions
ViewerWidget* ImageViewer::getViewerWidget(int tabId)
{
	QScrollArea* s = static_cast<QScrollArea*>(ui->tabWidget->widget(tabId));
	if (s) {
		ViewerWidget* vW = static_cast<ViewerWidget*>(s->widget());
		return vW;
	}
	return nullptr;
}
ViewerWidget* ImageViewer::getCurrentViewerWidget()
{
	return getViewerWidget(ui->tabWidget->currentIndex());
}

// Event filters
bool ImageViewer::eventFilter(QObject* obj, QEvent* event)
{
	if (obj->objectName() == "ViewerWidget") {
		return ViewerWidgetEventFilter(obj, event);
	}
	return false;
}

//ViewerWidget Events
bool ImageViewer::ViewerWidgetEventFilter(QObject* obj, QEvent* event)
{
	ViewerWidget* w = static_cast<ViewerWidget*>(obj);

	if (!w) {
		return false;
	}

	if (event->type() == QEvent::MouseButtonPress) {
		ViewerWidgetMouseButtonPress(w, event);
	}
	else if (event->type() == QEvent::MouseButtonRelease) {
		ViewerWidgetMouseButtonRelease(w, event);
	}
	else if (event->type() == QEvent::MouseMove) {
		ViewerWidgetMouseMove(w, event);
	}
	else if (event->type() == QEvent::Leave) {
		ViewerWidgetLeave(w, event);
	}
	else if (event->type() == QEvent::Enter) {
		ViewerWidgetEnter(w, event);
	}
	else if (event->type() == QEvent::Wheel) {
		ViewerWidgetWheel(w, event);
	}

	return QObject::eventFilter(obj, event);
}
void ImageViewer::ViewerWidgetMouseButtonPress(ViewerWidget* w, QEvent* event)
{
	QMouseEvent* e = static_cast<QMouseEvent*>(event);
	if (e->button() == Qt::LeftButton) {

		if (ui->ButtonNovy->isEnabled()) {	//ButtonNovy pouzivam na hodnotenie, ci sa kresli novy objekt alebo je nakresleny
			//teraz uz objekt je nakresleny
			SurCursor.resize(3);
			SurCursor[0] = e->pos();
		}
		else {	//objekt sa prave kresli
			if (Suradnice.isEmpty()) {		//klika sa prvy bod objektu
				Suradnice.push_back(e->pos());
			}
			else {
				Suradnice.push_back(e->pos());
				//POLYGON
				//ak chcem kreslit polygon, bude pridavat body do vektoru a postupne vykreslovat
				QVector<QPointF>TempUsecka = { Suradnice[(Suradnice.size() - 2)] ,  Suradnice[(Suradnice.size() - 1)] };

				if (ui->Bresenham->isChecked()) {
					getCurrentViewerWidget()->Orezanie(&TempUsecka, &PainterColor, 1);
				}
				else {
					getCurrentViewerWidget()->Orezanie(&TempUsecka, &PainterColor, 0);
				}
			}
		}
	}

	else if (e->button() == Qt::RightButton && !ui->ButtonNovy->isEnabled() && Suradnice.size() > 1) {
		//ak kliknem prave tlacidlo a kreslim polygon, tak ten polygon uzavriem

		QVector<QPointF>TempUsecka = { Suradnice[0] ,  Suradnice[(Suradnice.size() - 1)] };

		if (ui->Bresenham->isChecked()) {
			getCurrentViewerWidget()->Orezanie(&TempUsecka, &PainterColor, 1);
		}
		else {
			getCurrentViewerWidget()->Orezanie(&TempUsecka, &PainterColor, 0);
		}

		ui->ButtonNovy->setEnabled(true);		//ak dokreslim polygon, zastavi sa kreslenie
		ui->GroupTrans->setEnabled(true);
	}
}
void ImageViewer::ViewerWidgetMouseButtonRelease(ViewerWidget* w, QEvent* event)
{
	QMouseEvent* e = static_cast<QMouseEvent*>(event);

	if (ui->ButtonNovy->isEnabled() && SurCursor.size()==3 && e->button() == Qt::LeftButton) {
		int i;
		SurCursor[1] = e->pos();
		int DeltaX = SurCursor[1].x() - SurCursor[0].x();
		int DeltaY = SurCursor[1].y() - SurCursor[0].y();

		//zmena suradnic
		for (i = 0; i < Suradnice.size(); i++) {
			Suradnice[i].setX(Suradnice[i].x() + DeltaX);
			Suradnice[i].setY(Suradnice[i].y() + DeltaY);
		}

		getCurrentViewerWidget()->clear();

		if (ui->Bresenham->isChecked()) {
			getCurrentViewerWidget()->Orezanie(&Suradnice, &PainterColor, 1);
		}
		else {
			getCurrentViewerWidget()->Orezanie(&Suradnice, &PainterColor, 0);
		}

	}
}
void ImageViewer::ViewerWidgetMouseMove(ViewerWidget* w, QEvent* event)
{
	QMouseEvent* e = static_cast<QMouseEvent*>(event);

	if (ui->ButtonNovy->isEnabled() && SurCursor.size() == 3 && e->buttons() == Qt::LeftButton) {
		int i;
		SurCursor[1] = e->pos();
		int DeltaX = SurCursor[1].x() - SurCursor[0].x();
		int DeltaY = SurCursor[1].y() - SurCursor[0].y();

		QVector<QPointF>TempSur;

		//vytvorenie docasnych suradnic
		for (i = 0; i < Suradnice.size(); i++) {

			TempSur.push_back(QPoint(Suradnice[i].x() + DeltaX, Suradnice[i].y() + DeltaY));
			//Suradnice[i].setX(Suradnice[i].x() + DeltaX);
			//Suradnice[i].setY(Suradnice[i].y() + DeltaY);
		}

		getCurrentViewerWidget()->clear();

		if (ui->Bresenham->isChecked()) {
			getCurrentViewerWidget()->Orezanie(&TempSur, &PainterColor,1);
		}
		else {
			getCurrentViewerWidget()->Orezanie(&TempSur, &PainterColor,0);
		}
	}
}
void ImageViewer::ViewerWidgetLeave(ViewerWidget* w, QEvent* event)
{
}
void ImageViewer::ViewerWidgetEnter(ViewerWidget* w, QEvent* event)
{
}
void ImageViewer::ViewerWidgetWheel(ViewerWidget* w, QEvent* event)
{
	QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);

	//scale
	if (ui->ButtonNovy->isEnabled()) {
		int i;
		float ScaleValue;

		if (wheelEvent->angleDelta().y() > 0) {
			ScaleValue = 1.25;
		}
		else {
			ScaleValue = 0.75;
		}

		int BaseX = Suradnice[0].x();
		int BaseY = Suradnice[0].y();

		getCurrentViewerWidget()->clear();

		for (i = 1; i < Suradnice.size(); i++) {
			Suradnice[i].setX(BaseX + ((Suradnice[i].x() - BaseX) * ScaleValue));
			Suradnice[i].setY(BaseY + ((Suradnice[i].y() - BaseY) * ScaleValue));
		}

		if (ui->Bresenham->isChecked()) {
			getCurrentViewerWidget()->Orezanie(&Suradnice, &PainterColor, 1);
		}
		else {
			getCurrentViewerWidget()->Orezanie(&Suradnice, &PainterColor, 0);
		}
	}
}

//ImageViewer Events
void ImageViewer::closeEvent(QCloseEvent* event)
{
	if (QMessageBox::Yes == QMessageBox::question(this, "Close Confirmation", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No))
	{
		event->accept();
	}
	else {
		event->ignore();
	}
}

//Image functions
void ImageViewer::openNewTabForImg(ViewerWidget* vW)
{
	QScrollArea* scrollArea = new QScrollArea;
	scrollArea->setWidget(vW);

	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidgetResizable(true);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	vW->setObjectName("ViewerWidget");
	vW->installEventFilter(this);

	QString name = vW->getName();

	ui->tabWidget->addTab(scrollArea, name);
}
bool ImageViewer::openImage(QString filename)
{
	QFileInfo fi(filename);

	QString name = fi.baseName();
	openNewTabForImg(new ViewerWidget(name, QSize(0, 0)));
	ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);

	ViewerWidget* w = getCurrentViewerWidget();

	QImage loadedImg(filename);
	return w->setImage(loadedImg);
}
bool ImageViewer::saveImage(QString filename)
{
	QFileInfo fi(filename);
	QString extension = fi.completeSuffix();
	ViewerWidget* w = getCurrentViewerWidget();

	QImage* img = w->getImage();
	return img->save(filename, extension.toStdString().c_str());
}
void ImageViewer::clearImage()
{
	ViewerWidget* w = getCurrentViewerWidget();
	w->clear();
}
void ImageViewer::setBackgroundColor(QColor color)
{
	ViewerWidget* w = getCurrentViewerWidget();
	w->clear(color);
}

//Slots

//Tabs slots
void ImageViewer::on_tabWidget_tabCloseRequested(int tabId)
{
	ViewerWidget* vW = getViewerWidget(tabId);
	delete vW; //vW->~ViewerWidget();
	ui->tabWidget->removeTab(tabId);

	if (tabId == 0) {
		ui->groupBox->setEnabled(false);
	}
}
void ImageViewer::on_actionRename_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}
	ViewerWidget* w = getCurrentViewerWidget();
	bool ok;
	QString text = QInputDialog::getText(this, QString("Rename image"), tr("Image name:"), QLineEdit::Normal, w->getName(), &ok);
	if (ok && !text.trimmed().isEmpty())
	{
		w->setName(text);
		ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), text);
	}
}

//Image slots
void ImageViewer::on_actionNew_triggered()
{
	newImgDialog = new NewImageDialog(this);
	connect(newImgDialog, SIGNAL(accepted()), this, SLOT(newImageAccepted()));
	newImgDialog->exec();
}
void ImageViewer::newImageAccepted()
{
	NewImageDialog* newImgDialog = static_cast<NewImageDialog*>(sender());

	int width = newImgDialog->getWidth();
	int height = newImgDialog->getHeight();
	QString name = newImgDialog->getName();
	openNewTabForImg(new ViewerWidget(name, QSize(width, height)));
	ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
	ui->groupBox->setEnabled(true);

	if (ui->Bresenham->isChecked()) {
		getCurrentViewerWidget()->Orezanie(&Suradnice, &PainterColor, 1);
	}
	else {
		getCurrentViewerWidget()->Orezanie(&Suradnice, &PainterColor, 0);
	}
}
void ImageViewer::on_actionOpen_triggered()
{
	QString folder = settings.value("folder_img_load_path", "").toString();

	QString fileFilter = "Image data (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm .*xbm .* xpm);;All files (*)";
	QString fileName = QFileDialog::getOpenFileName(this, "Load image", folder, fileFilter);
	if (fileName.isEmpty()) { return; }

	QFileInfo fi(fileName);
	settings.setValue("folder_img_load_path", fi.absoluteDir().absolutePath());

	if (!openImage(fileName)) {
		msgBox.setText("Unable to open image.");
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
	}
}
void ImageViewer::on_actionSave_as_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image to save.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}
	QString folder = settings.value("folder_img_save_path", "").toString();

	ViewerWidget* w = getCurrentViewerWidget();

	QString fileFilter = "Image data (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm .*xbm .* xpm);;All files (*)";
	QString fileName = QFileDialog::getSaveFileName(this, "Save image", folder + "/" + w->getName(), fileFilter);
	if (fileName.isEmpty()) { return; }

	QFileInfo fi(fileName);
	settings.setValue("folder_img_save_path", fi.absoluteDir().absolutePath());

	if (!saveImage(fileName)) {
		msgBox.setText("Unable to save image.");
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
	}
	else {
		msgBox.setText(QString("File %1 saved.").arg(fileName));
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
	}
}
void ImageViewer::on_actionClear_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}
	clearImage();
}
void ImageViewer::on_actionSet_background_color_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}

	QColor backgroundColor = QColorDialog::getColor(Qt::white, this, "Select color of background");
	if (backgroundColor.isValid()) {
		setBackgroundColor(backgroundColor);
	}
}

void ImageViewer::on_Farba_pressed()
{
	QColor selectedColor = QColorDialog::getColor(Qt::white, this, "Select color of background");
	if (selectedColor.isValid()) {
		QString style = "background-color: rgb(%1, %2, %3);";
		ui->Farba->setStyleSheet(style.arg(selectedColor.red()).arg(selectedColor.green()).arg(selectedColor.blue()));
		PainterColor = selectedColor;
	}
}

//UI Buttons
void ImageViewer::on_ButtonNovy_pressed()
{
	Suradnice.clear();		//vymaze data z vektora
	Suradnice.squeeze();	//uvolni pamat
	SurCursor.clear();
	SurCursor.squeeze();

	ui->ButtonNovy->setEnabled(false);
	ui->GroupTrans->setEnabled(false);
	getCurrentViewerWidget()->clear();
}
void ImageViewer::on_ButtonRot_pressed()
{
	//rot
	if (remainder(ui->SpinRot->value(),360.0) != 0.0) {		//testuje, ci sa vobec ma otacat, ci uhol rotovania nie je nasobkom 360
		int i;
		double RotAngle = ui->SpinRot->value() / 180.0 * M_PI;
		double BaseX = Suradnice[0].x();
		double BaseY = Suradnice[0].y();

		getCurrentViewerWidget()->clear();

		for (i = 1; i < Suradnice.size(); i++) {
			int CurrX = Suradnice[i].x();
			int CurrY = Suradnice[i].y();
			Suradnice[i].setX((CurrX - BaseX) * qCos(RotAngle) + (CurrY - BaseY) * qSin(RotAngle) + BaseX);
			Suradnice[i].setY(-(CurrX - BaseX) * qSin(RotAngle) + (CurrY - BaseY) * qCos(RotAngle) + BaseY);
		}

		if (ui->Bresenham->isChecked()) {
			getCurrentViewerWidget()->Orezanie(&Suradnice, &PainterColor, 1);
		}
		else {
			getCurrentViewerWidget()->Orezanie(&Suradnice, &PainterColor, 0);
		}
	}
}
void ImageViewer::on_ButtonScale_pressed()
{
	//scale
	int i;
	double ScaleValue = ui->SpinScale->value();
	double BaseX = Suradnice[0].x();
	double BaseY = Suradnice[0].y();

	getCurrentViewerWidget()->clear();

	for (i = 1; i < Suradnice.size(); i++) {
		Suradnice[i].setX(BaseX + ((Suradnice[i].x() - BaseX) * ScaleValue));
		Suradnice[i].setY(BaseY + ((Suradnice[i].y() - BaseY) * ScaleValue));
	}

	if (ui->Bresenham->isChecked()) {
		getCurrentViewerWidget()->Orezanie(&Suradnice, &PainterColor, 1);
	}
	else {
		getCurrentViewerWidget()->Orezanie(&Suradnice, &PainterColor, 0);
	}
} 
void ImageViewer::on_ButtonShear_pressed()
{
	//shear
	int i;
	double ShearValue = ui->SpinShear->value() / 10.0;	//delim 10, aby sa s tym narabalo pohodlnejsie

	for (i = 1; i < Suradnice.size(); i++) {
		Suradnice[i].setX(Suradnice[i].x() + ShearValue * (Suradnice[i].y() - Suradnice[0].y()));
	}

	getCurrentViewerWidget()->clear();

	if (ui->Bresenham->isChecked()) {
		getCurrentViewerWidget()->Orezanie(&Suradnice, &PainterColor, 1);
	}
	else {
		getCurrentViewerWidget()->Orezanie(&Suradnice, &PainterColor, 0);
	}
}
void ImageViewer::on_ButtonSymm_pressed()
{
	//symmetry
	if (Suradnice.size() > 2) {		//polygon
		int i;
		double u = Suradnice[1].x() - Suradnice[0].x();
		double v = Suradnice[1].y() - Suradnice[0].y();

		double a = v;
		double b = -u;
		double c = -a * Suradnice[0].x() - b * Suradnice[0].y();

		for (i = 2; i < Suradnice.size(); i++) {
			double BaseX = Suradnice[i].x();
			double BaseY = Suradnice[i].y();
			Suradnice[i].setX(BaseX - 2 * a * (a * BaseX + b * BaseY + c) / (a * a + b * b));
			Suradnice[i].setY(BaseY - 2 * b * (a * BaseX + b * BaseY + c) / (a * a + b * b));
		}

		getCurrentViewerWidget()->clear();

		if (ui->Bresenham->isChecked()) {
			getCurrentViewerWidget()->Orezanie(&Suradnice, &PainterColor, 1);
		}
		else {
			getCurrentViewerWidget()->Orezanie(&Suradnice, &PainterColor, 0);
		}
	}
	else {		//usecka
		Suradnice[1].setY(Suradnice[0].y() - (Suradnice[1].y() - Suradnice[0].y()));		//symetria cez os x
		getCurrentViewerWidget()->clear();
		
		if (ui->Bresenham->isChecked()) {
			getCurrentViewerWidget()->Orezanie(&Suradnice, &PainterColor, 1);
		}
		else {
			getCurrentViewerWidget()->Orezanie(&Suradnice, &PainterColor, 0);
		}
	}
}

//UI Nastavenia
void ImageViewer::on_SpinRot_valueChanged(double)
{
	ui->SliderRot->setValue(round(ui->SpinRot->value()));
}
void ImageViewer::on_SliderRot_valueChanged(int)
{
	ui->SpinRot->setValue(ui->SliderRot->value());
}

void ImageViewer::on_SpinScale_valueChanged(double)
{
	ui->SliderScale->setValue(round(ui->SpinScale->value()));
}
void ImageViewer::on_SliderScale_valueChanged(int)
{
	ui->SpinScale->setValue(ui->SliderScale->value());
}

void ImageViewer::on_SpinShear_valueChanged(double)
{
	ui->SliderShear->setValue(round(ui->SpinShear->value()));
}
void ImageViewer::on_SliderShear_valueChanged(int)
{
	ui->SpinShear->setValue(ui->SliderShear->value());
}
