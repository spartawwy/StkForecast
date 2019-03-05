#include "code_list_wall.h"

#include <QStandardItemModel>
#include <qevent.h> 

#include <TLib/core/tsystem_utility_functions.h>

CodeListWall::CodeListWall(StkForecastApp *app, QWidget *parent) : app_(app)
{
    ui.setupUi(this);

    // init ui -----------
    QPalette pal = this->palette();
    pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

bool CodeListWall::Init()
{
    ui.tbview_codes->setContextMenuPolicy(Qt::CustomContextMenu);
    ui.tbview_codes->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tbview_codes->setSelectionMode(QAbstractItemView::SingleSelection);

    QStandardItemModel * model = new QStandardItemModel(0, 2/*cols*/, this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString::fromLocal8Bit("ДњТы")));
    model->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignCenter);
    ui.tbview_codes->setModel(model);
    
    model->insertRow(model->rowCount());
    int row_index = model->rowCount() - 1;
    auto align_way = Qt::AlignCenter;
    auto item = new QStandardItem( TSystem::utility::FormatStr("%d",  600196).c_str());
    model->setItem(row_index, 0, item);
    model->item(row_index, 0)->setTextAlignment(align_way);
	ui.tbview_codes->setColumnWidth(0, 100);

    return true;
}


void CodeListWall::keyPressEvent(QKeyEvent *e) 
{
    e->ignore();
}