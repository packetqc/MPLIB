
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

void ScreenLogsPresenter::setButtonMode() {
	view.setButtonMode();
}

//void ScreenLogsPresenter::setColor() {
//	view.setColor();
//}

void ScreenLogsPresenter::SetDisplayColor()
{
	view.SetDisplayColor();
}

void ScreenLogsPresenter::updateStatus_STORAGE(uint8_t value) {
	view.updateStatus_STORAGE(value);
}

void ScreenLogsPresenter::UpdateStatusNavigationBar() {
	view.UpdateStatusNavigationBar();
}

void ScreenLogsPresenter::UpdateLogs(uint8_t index) {
	view.UpdateLogs(index);
}

void ScreenLogsPresenter::getMemHeap() {
	view.getMemHeap();
}

void ScreenLogsPresenter::getNumberItemsList() {
	view.getNumberItemsList();
}

void ScreenLogsPresenter::getNumberOfLogListDrawables() {
	view.getNumberOfLogListDrawables();
}

void ScreenLogsPresenter::getNumberLogsDataServices() {
	view.getNumberLogsDataServices();
}

uint32_t ScreenLogsPresenter::getColorMode() {
	return model->getColorMode();
}

void ScreenLogsPresenter::setColorMode(uint32_t mode)
{
	model->setColorMode(mode);
}

void ScreenLogsPresenter::updateBackground()
{
	view.updateBackground();
}

