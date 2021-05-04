//
// Created by vhund on 01.03.2021.
//

#ifndef DB_QT_COURSEWORK_MAINWINDOW_HXX
#define DB_QT_COURSEWORK_MAINWINDOW_HXX

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <iostream>

#include "a_star.hpp"
#include "matrix.hpp"
#include "node.hpp"

class MainWindow : public QMainWindow {
  Q_OBJECT;

 public:
  explicit MainWindow(QPair<int, int> t_screenSize) {
	initLog();
	screenSize = {t_screenSize.first, t_screenSize.second};
	setFixedSize(screenSize.first / 2 + screenSize.first / 4, screenSize.second / 2);
	cellSize=80;
	ResizeGrid();

	SetLocation();
	setMouseTracking(true);
	aStar.SetGrid(grid);

	gridLayout = new QGridLayout();
	widget = new QWidget(this);
	widget->setLayout(gridLayout);
	setCentralWidget(widget);

	addWall_btn = new QPushButton(this);
	addWall_btn->setText("Add/Remove Walls");
	setStart_btn = new QPushButton(this);
	setStart_btn->setText("Set start");
	setEnd_btn = new QPushButton(this);
	setEnd_btn->setText("Set end");

	QFont font = addWall_btn->font();
	font.setPixelSize(16);

	addWall_btn->setFont(font);
	setStart_btn->setFont(font);
	setEnd_btn->setFont(font);
	spacer = new QSpacerItem(screenSize.first / 2 + screenSize.first / 8, screenSize.second);

	gridLayout->addItem(spacer, 0, 0, 3);
	gridLayout->addWidget(addWall_btn, 0, 1);
	gridLayout->addWidget(setStart_btn, 1, 1);
	gridLayout->addWidget(setEnd_btn, 2, 1);

	statusLabel = new QLabel();
	font.setPixelSize(24);
	statusLabel->setFont(font);
	gridLayout->addWidget(statusLabel, 5, 1);
	aStar.aStar();
	changeStatus(aStar.GetStatus());

	connect(addWall_btn, &QPushButton::clicked, this, [this]() {
	  if (state != 'w') state = 'w';
	  else
		state = 'u';
	});
	connect(setStart_btn, &QPushButton::clicked, this, [this]() {
	  if (state != 's') state = 's';
	  else
		state = 'u';
	});
	connect(setEnd_btn, &QPushButton::clicked, this, [this]() {
	  if (state != 'e') state = 'e';
	  else
		state = 'u';
	});
  }

 private:
  matrix<Node> *grid;
  A_Star aStar;
  QWidget *widget;
  QGridLayout *gridLayout;
  QPushButton *setStart_btn;
  QPushButton *setEnd_btn;
  QPushButton *addWall_btn;
  QSpacerItem *spacer;
  QLabel *statusLabel;
  int cellSize{30};
  vec2i screenSize{0, 0};

  /**
   * s - Set start point
   * e - Set end point
   * i - Start node blocked by walls
   * b - End node blocked by walls
   * f - Failed to find path
 * */
  char status = 'u';
  /**
   * w - Setting walls
   * s - Setting start point
   * e - Setting end point
   */
  char state = 'u';

  static void initLog() {
	// create color multi threaded logger
	auto console = spdlog::stdout_color_mt("console");
	auto err_logger = spdlog::stderr_color_mt("stderr");
  }

  void mousePressEvent(QMouseEvent *event) override {
	if (event->buttons() == Qt::LeftButton) {
	  if (aStar.isValid({event->pos().y() / cellSize, event->pos().x() / cellSize})) {
		aStar.ResetVisitedNodes();
		switch (state) {
		  case 'w':
			if (grid->get(event->pos().y() / cellSize, event->pos().x() / cellSize).GetType() == -1) {
			  grid->get(event->pos().y() / cellSize, event->pos().x() / cellSize).SetType(0);
			} else {
			  grid->get(event->pos().y() / cellSize, event->pos().x() / cellSize).SetType(-1);
			}
			break;
		  case 's':
			if (aStar.getStartNode() != nullptr) {
			  aStar.getStartNode()->SetType(0);
			}
			grid->get(event->pos().y() / cellSize, event->pos().x() / cellSize).SetType(1);
			break;
		  case 'e':
			if (aStar.getEndNode() != nullptr) {
			  aStar.getEndNode()->SetType(0);
			}
			grid->get(event->pos().y() / cellSize, event->pos().x() / cellSize).SetType(2);
			break;
		  default:
			break;
		}
		ResetGraph();
		aStar.aStar();
		changeStatus(aStar.GetStatus());
		update();
	  }
	}
  }

  void changeStatus(char newStatus) {
	status = newStatus;
	switch (status) {
	  case 's':
		statusLabel->setStyleSheet(QStringLiteral("QLabel{color: rgb(200, 0, 0);}"));
		statusLabel->setText("Set start point");
		break;
	  case 'e':
		statusLabel->setStyleSheet(QStringLiteral("QLabel{color: rgb(200, 0, 0);}"));
		statusLabel->setText("Set end point");
		break;
	  case 'i':
		statusLabel->setStyleSheet(QStringLiteral("QLabel{color: rgb(200, 0, 0);}"));
		statusLabel->setText("Start node blocked by walls");
		break;
	  case 'b':
		statusLabel->setStyleSheet(QStringLiteral("QLabel{color: rgb(200, 0, 0);}"));
		statusLabel->setText("End node blocked by walls!");
		break;
	  case 'f':
		statusLabel->setStyleSheet(QStringLiteral("QLabel{color: rgb(200, 0, 0);}"));
		statusLabel->setText("Failed to find path");
		break;
	  default:
		statusLabel->setText("");
	}
  }

  void paintEvent(QPaintEvent *event) override {
	Q_UNUSED(event);
	QPainter painter(this);// Create object of QPainter
	for (int row = 0; row < grid->rows(); ++row) {
	  for (int column = 0; column < grid->columns(); ++column) {
		switch (grid->get(row, column).GetType()) {
		  case -1:
			painter.setBrush(QBrush(QColor(82, 82, 82), Qt::SolidPattern));
			painter.drawRect(column * cellSize, row * cellSize, cellSize, cellSize);
			break;
		  case 0:
			painter.setBrush(QBrush(QColor(54, 54, 54), Qt::SolidPattern));
			painter.drawRect(column * cellSize, row * cellSize, cellSize, cellSize);
			break;
		  case 1:
			painter.setBrush(QBrush(QColor(91, 181, 138), Qt::SolidPattern));
			painter.drawRect(column * cellSize, row * cellSize, cellSize, cellSize);
			break;
		  case 2:
			painter.setBrush(QBrush(QColor(133, 91, 181), Qt::SolidPattern));
			painter.drawRect(column * cellSize, row * cellSize, cellSize, cellSize);
			break;
		  case 3:
			painter.setBrush(QBrush(QColor(91, 145, 181), Qt::SolidPattern));
			painter.drawRect(column * cellSize, row * cellSize, cellSize, cellSize);
			break;
		  default:
			break;
		}
		if(cellSize>75){
		  QFont font=painter.font();
		  font.setPixelSize(18);
		  painter.setFont(font);


		  if(grid->get(row, column).GetG()==std::numeric_limits<double>::max()) {
			painter.drawText(column * cellSize, row * cellSize+cellSize/3, "G: ∞");
		  }
		  else{
			painter.drawText(column * cellSize, row * cellSize+cellSize/3,"G: "+QString::number(grid->get(row, column).GetG(), 'G', 2));
		  }

		  if(grid->get(row, column).GetH()==std::numeric_limits<double>::max()) {
			painter.drawText(column * cellSize, row * cellSize+(cellSize*2/3), "H: ∞");
		  }
		  else{
			painter.drawText(column * cellSize, row * cellSize+cellSize*2/3,"H: "+QString::number(grid->get(row, column).GetH(), 'G', 2));
		  }

		  if(grid->get(row, column).GetF()==std::numeric_limits<double>::max()) {
			painter.drawText(column * cellSize, row * cellSize+cellSize, "F: ∞");
		  }
		  else{
			painter.drawText(column * cellSize, row * cellSize+cellSize,"F: "+QString::number(grid->get(row, column).GetF(), 'G', 2));
		  }
		}
	  }
	}
  }
  void SetLocation() {
	for (int row = 0; row < grid->rows(); ++row) {
	  for (int column = 0; column < grid->columns(); ++column) {
		grid->get(row, column).position = {row, column};
	  }
	}
  }

  void ResetGraph() {
	for (int row = 0; row < grid->rows(); ++row) {
	  for (int column = 0; column < grid->columns(); ++column) {
		grid->get(row, column).SetG(std::numeric_limits<double>::max());
		grid->get(row, column).SetH(std::numeric_limits<double>::max());
		grid->get(row, column).SetF(std::numeric_limits<double>::max());
		grid->get(row, column).UpdateParent(nullptr);
	  }
	}
  }
  void ResizeGrid() {
	grid = new matrix<Node>(screenSize.second / (cellSize * 2), (screenSize.first / 2 + screenSize.first / 8) / cellSize);
	ResetGraph();
	SetLocation();
	if (aStar.getStartNode() != nullptr) {
	  aStar.getStartNode()->SetType(0);
	}
	if (aStar.getEndNode() != nullptr) {
	  aStar.getEndNode()->SetType(0);
	}
	aStar.SetGrid(grid);
	update();
  }
};

#endif//DB_QT_COURSEWORK_MAINWINDOW_HXX
