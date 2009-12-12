/*
 * Copyright (c) 2009 Sean C. Rhea (srhea@srhea.net)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "PfPvWindow.h"
#include "MainWindow.h"
#include "PfPvPlot.h"
#include "RideItem.h"
#include <QtGui>

PfPvWindow::PfPvWindow(MainWindow *mainWindow) :
    QWidget(mainWindow), mainWindow(mainWindow)
{
    QVBoxLayout *vlayout = new QVBoxLayout;
    QHBoxLayout *qaLayout = new QHBoxLayout;

    pfPvPlot = new PfPvPlot();
    QLabel *qaCPLabel = new QLabel(tr("Watts:"), this);
    qaCPValue = new QLineEdit(QString("%1").arg(pfPvPlot->getCP()));
    qaCPValue->setValidator(new QIntValidator(0, 9999, qaCPValue));
    QLabel *qaCadLabel = new QLabel(tr("RPM:"), this);
    qaCadValue = new QLineEdit(QString("%1").arg(pfPvPlot->getCAD()));
    qaCadValue->setValidator(new QIntValidator(0, 999, qaCadValue));
    QLabel *qaClLabel = new QLabel(tr("Crank Length (m):"), this);
    qaClValue = new QLineEdit(QString("%1").arg(1000 * pfPvPlot->getCL()));
    shadeZonesPfPvCheckBox = new QCheckBox;
    shadeZonesPfPvCheckBox->setText("Shade zones");
    shadeZonesPfPvCheckBox->setCheckState(Qt::Checked);
    mergeIntervalPfPvCheckBox = new QCheckBox;
    mergeIntervalPfPvCheckBox->setText("Merge intervals");
    mergeIntervalPfPvCheckBox->setCheckState(Qt::Unchecked);

    qaLayout->addWidget(qaCPLabel);
    qaLayout->addWidget(qaCPValue);
    qaLayout->addWidget(qaCadLabel);
    qaLayout->addWidget(qaCadValue);
    qaLayout->addWidget(qaClLabel);
    qaLayout->addWidget(qaClValue);
    qaLayout->addWidget(shadeZonesPfPvCheckBox);
    qaLayout->addWidget(mergeIntervalPfPvCheckBox);

    vlayout->addWidget(pfPvPlot);
    vlayout->addLayout(qaLayout);
    setLayout(vlayout);

    connect(pfPvPlot, SIGNAL(changedCP(const QString&)),
            qaCPValue, SLOT(setText(const QString&)) );
    connect(pfPvPlot, SIGNAL(changedCAD(const QString&)),
            qaCadValue, SLOT(setText(const QString&)) );
    connect(pfPvPlot, SIGNAL(changedCL(const QString&)),
            qaClValue, SLOT(setText(const QString&)) );
    connect(qaCPValue, SIGNAL(editingFinished()),
	    this, SLOT(setQaCPFromLineEdit()));
    connect(qaCadValue, SIGNAL(editingFinished()),
	    this, SLOT(setQaCADFromLineEdit()));
    connect(qaClValue, SIGNAL(editingFinished()),
	    this, SLOT(setQaCLFromLineEdit()));
    connect(shadeZonesPfPvCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(setShadeZonesPfPvFromCheckBox()));
    connect(mergeIntervalPfPvCheckBox, SIGNAL(stateChanged(int)),
                this, SLOT(setMergeIntervalsPfPvFromCheckBox()));
    connect(mainWindow, SIGNAL(rideSelected()), this, SLOT(rideSelected()));
    connect(mainWindow, SIGNAL(intervalSelected()), this, SLOT(intervalSelected()));
    connect(mainWindow, SIGNAL(zonesChanged()), this, SLOT(zonesChanged()));
}

void
PfPvWindow::rideSelected()
{
    RideItem *ride = mainWindow->rideItem();
    if (!ride)
        return;
    pfPvPlot->setData(ride);
    // update the QLabel widget with the CP value set in PfPvPlot::setData()
    qaCPValue->setText(QString("%1").arg(pfPvPlot->getCP()));
}

void
PfPvWindow::intervalSelected()
{
    RideItem *ride = mainWindow->rideItem();
    if (!ride) return;
    pfPvPlot->setData(ride);

}

void
PfPvWindow::zonesChanged()
{
    pfPvPlot->refreshZoneItems();
    pfPvPlot->replot();
    qaCPValue->setText(QString("%1").arg(pfPvPlot->getCP()));
}

void
PfPvWindow::setShadeZonesPfPvFromCheckBox()
{
    if (pfPvPlot->shadeZones() != shadeZonesPfPvCheckBox->isChecked()) {
        pfPvPlot->setShadeZones(shadeZonesPfPvCheckBox->isChecked());
    }
}

void
PfPvWindow::setMergeIntervalsPfPvFromCheckBox()
{
    if (pfPvPlot->mergeIntervals() != mergeIntervalPfPvCheckBox->isChecked()) {
        pfPvPlot->setMergeIntervals(mergeIntervalPfPvCheckBox->isChecked());
    }
}

void
PfPvWindow::setQaCPFromLineEdit()
{
    int value = qaCPValue->text().toInt();
    pfPvPlot->setCP(value);
}

void
PfPvWindow::setQaCADFromLineEdit()
{
    int value = qaCadValue->text().toInt();
    pfPvPlot->setCAD(value);
}

void
PfPvWindow::setQaCLFromLineEdit()
{
    double value = qaClValue->text().toDouble();
    pfPvPlot->setCL(value);
}


