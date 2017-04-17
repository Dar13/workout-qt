#ifndef COUNTDOWNMODEL_H
#define COUNTDOWNMODEL_H

#include <QObject>
#include <QTimer>
#include <QPushButton>
#include <QPlainTextEdit>

class CountdownModel : public QObject
{
    Q_OBJECT

    enum CountdownModelState
    {
        WAITING,
        IN_PROGRESS,
        TIMEOUT
    };

public:
    explicit CountdownModel(QObject *parent = 0);

signals:

public slots:
    void countdownTimeout();
    void handleButton(bool);

private:
    bool _started;
    CountdownModelState _state;
    QPushButton* _button;
    QPlainTextEdit* _value;
    QTimer* _timer;
};

#endif // COUNTDOWNMODEL_H
