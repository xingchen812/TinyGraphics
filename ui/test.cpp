#include <tg/base.h>

namespace tg {
class Graphics2D : public Window {
public:
	Graphics2D()
		: scale(1.0), offsetX(0), offsetY(0), dragging(false) {
		// setScene(new QGraphicsScene());
		// scene()->addItem(&item_);
		// setAttribute(Qt::WA_StaticContents);
		// setFixedSize(800, 600);
		// setWindowTitle(title);
	}

protected:
	void paintEvent(QPaintEvent* event) override {
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);

		// Apply scaling and translation
		painter.translate(width() / 2 + offsetX, height() / 2 + offsetY);
		painter.scale(scale, -scale); // Invert y-axis to match conventional coordinate system

		// Draw grid
		QPen gridPen(Qt::gray, 1, Qt::DotLine);
		painter.setPen(gridPen);

		for (int i = -width() / 2; i < width() / 2; i += 20) {
			painter.drawLine(i, -height() / 2, i, height() / 2);
		}
		for (int i = -height() / 2; i < height() / 2; i += 20) {
			painter.drawLine(-width() / 2, i, width() / 2, i);
		}

		// Draw axes
		QPen axisPen(Qt::black, 2);
		painter.setPen(axisPen);
		painter.drawLine(-width() / 2, 0, width() / 2, 0);
		painter.drawLine(0, -height() / 2, 0, height() / 2);

		// Draw origin point
		painter.setBrush(Qt::black);
		painter.drawEllipse(QPointF(0, 0), 5, 5);
	}

	void mousePressEvent(QMouseEvent* event) override {
		if (event->button() == Qt::LeftButton) {
			dragging = true;
			lastPos = event->pos();
		}
	}

	void mouseReleaseEvent(QMouseEvent* event) override {
		if (event->button() == Qt::LeftButton) {
			dragging = false;
		}
	}

	void mouseMoveEvent(QMouseEvent* event) override {
		if (dragging) {
			QPoint delta = event->pos() - lastPos;
			offsetX += delta.x();
			offsetY += delta.y();
			lastPos = event->pos();
			update();
		}
	}

	void wheelEvent(QWheelEvent* event) override {
		if (event->angleDelta().y() != 0) {
			double factor = 1.1;
			if (event->angleDelta().y() < 0) {
				factor = 1.0 / factor;
			}
			scale *= factor;
			update();
		}
	}

private:
	double scale;
	int offsetX;
	int offsetY;
	bool dragging;
	QPoint lastPos;
};
TG_QUICK_WINDOW_REGISTER(Graphics2D);
} // namespace tg
