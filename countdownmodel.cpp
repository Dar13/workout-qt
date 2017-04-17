#include "countdownmodel.h"

CountdownModel::CountdownModel(QObject *parent)
    : QObject(parent),
      _started(false),
      _state(WAITING),
      _button(parent->findChild<QPushButton*>(tr("countdown_control"))),
      _value(parent->findChild<QPlainTextEdit*>(tr("countdown_value"))),
      _timer(nullptr)
{
    connect(_button, SIGNAL(clicked(bool)), this, SLOT(handleButton(bool)));
}

void CountdownModel::countdownTimeout()
{
    // Handle the timeout (notification, change button, etc)
    _state = TIMEOUT;

    _button->setText(tr("Done!"));
}

void CountdownModel::handleButton(bool)
{
    switch(_state)
    {
    case WAITING:
    {
        bool convert = false;
        _state = IN_PROGRESS;
        _button->setText(tr("Pause"));

        // Timer text edit is in seconds
        int timer_msec = _value->toPlainText().toInt(&convert) * 1000;
        if(_timer)
        {
            delete _timer;
        }

        _timer = new QTimer(nullptr);
        connect(_timer, SIGNAL(timeout()), this, SLOT(countdownTimeout()));
        _timer->start(timer_msec);
    }
        break;
    case IN_PROGRESS:
        _state = WAITING;
        if(_timer)
        {
            delete _timer;
            _timer = nullptr;
        }
        break;
    case TIMEOUT:
        _state = WAITING;
        if(_timer)
        {
            delete _timer;
            _timer = nullptr;
        }
        break;
    }
}
