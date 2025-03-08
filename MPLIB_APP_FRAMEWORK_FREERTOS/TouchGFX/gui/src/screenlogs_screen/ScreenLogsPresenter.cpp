#include <gui/screenlogs_screen/ScreenLogsView.hpp>
#include <gui/screenlogs_screen/ScreenLogsPresenter.hpp>

ScreenLogsPresenter::ScreenLogsPresenter(ScreenLogsView& v)
    : view(v)
{

}

void ScreenLogsPresenter::activate()
{

}

void ScreenLogsPresenter::deactivate()
{

}

void ScreenLogsPresenter::UpdateStatusNavigationBar() {
//	view.UpdateStatusNavigationBar();
}

void ScreenLogsPresenter::UpdateLogs(uint8_t index) {
	view.UpdateLogs(index);
}

void ScreenLogsPresenter::getMemHeap() {
//	view.getMemHeap();
}

void ScreenLogsPresenter::getNumberItemsList() {
	view.getNumberItemsList();
}

void ScreenLogsPresenter::getNumberOfLogListDrawables() {
//	view.getNumberOfLogListDrawables();
}

void ScreenLogsPresenter::getNumberLogsDataServices() {
	view.getNumberLogsDataServices();
}
