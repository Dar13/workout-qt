#include "countdownmodel.h"

CountdownModel::CountdownModel(QObject *parent)
    : QObject(parent),
      _count(0),
      _started(false),
      _state(WAITING),
      _button(parent->findChild<QPushButton*>(tr("countdown_control"))),
      _value(parent->findChild<QPlainTextEdit*>(tr("countdown_value"))),
      _timer(new QTimer(parent))
{
    connect(_button, SIGNAL(clicked(bool)), this, SLOT(handleButton(bool)));

    _timer->setInterval(1000);
    _timer->start();
    connect(_timer, SIGNAL(timeout()), this, SLOT(countdownPeriodic()));
}

CountdownModel::~CountdownModel()
{
    delete _timer;
}

void CountdownModel::countdownPeriodic()
{
    if(_state == IN_PROGRESS)
    {
        _count--;
        _value->document()->setPlainText(tr("%0").arg(_count));
        if(_count == 0)
        {
            _state = TIMEOUT;
            _updateState();
        }
    }
}

void CountdownModel::_updateState()
{
    switch(_state)
    {
    case WAITING:
        _button->setText(tr("Start"));
        break;
    case IN_PROGRESS:
        _button->setText(tr("Pause"));
        break;
    case TIMEOUT:
        _button->setText(tr("Reset"));
        break;
    }
}

void CountdownModel::handleButton(bool)
{
    switch(_state)
    {
    case WAITING:
    {
        _state = IN_PROGRESS;

        bool converted = true;
        _count = _value->document()->toPlainText().toInt(&converted);
        _initialCount = _count;
        if(!converted)
        {
            // TODO: Something, failed to convert
        }
    }
        break;
    case IN_PROGRESS:
        _state = WAITING;
        break;
    case TIMEOUT:
        _state = WAITING;
        _value->document()->setPlainText(tr("%0").arg(_initialCount));
        break;
    }

    _updateState();
}
