#include <veutil/qt/ve_qitem.hpp>
#include "ve_qitem_init_monitor.h"

VeQItemInitMonitor::VeQItemInitMonitor(QObject *parent):
	QObject(parent)
{
}

void VeQItemInitMonitor::addItem(VeQItem *item)
{
	if (item->isLeaf()) {
		mItems.append(item);
	} else {
		for (int i=0;;++i) {
			VeQItem *child = item->itemChild(i);
			if (child == 0)
				break;
			addItem(child);
		}
	}
}

void VeQItemInitMonitor::start()
{
	if (checkState()) {
		emit initialized();
		return;
	}
	foreach (VeQItem *item, mItems) {
		connect(item, SIGNAL(stateChanged(VeQItem::State)),
				this, SLOT(onStateChanged()));
		if (item->getState() == VeQItem::Idle)
			item->getValue();
	}
}

bool VeQItemInitMonitor::checkState()
{
	if (mItems.isEmpty())
		return false;
	foreach (VeQItem *item, mItems) {
		VeQItem::State state = item->getState();
		if (state == VeQItem::Idle || state == VeQItem::Requested)
			return false;
	}
	return true;
}

void VeQItemInitMonitor::monitor(VeQItem *item, QObject *dest, const char *slot,
								 Qt::ConnectionType connectionType)
{
	VeQItemInitMonitor *monitor = new VeQItemInitMonitor(dest);
	connect(monitor, SIGNAL(initialized()), dest, slot, connectionType);
	connect(monitor, SIGNAL(initialized()), monitor, SLOT(deleteLater()), connectionType);
	monitor->addItem(item);
	monitor->start();
}

void VeQItemInitMonitor::monitor(const QList<VeQItem *> &items, QObject *dest, const char *slot,
								 Qt::ConnectionType connectionType)
{
	VeQItemInitMonitor *monitor = new VeQItemInitMonitor(dest);
	connect(monitor, SIGNAL(initialized()), dest, slot, connectionType);
	connect(monitor, SIGNAL(initialized()), monitor, SLOT(deleteLater()), connectionType);
	foreach(VeQItem *item, items)
		monitor->addItem(item);
	monitor->start();
}

void VeQItemInitMonitor::onStateChanged()
{
	if (checkState()) {
		mItems.clear();
		emit initialized();
	}
}
