#include "widgets/buttonlabel.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

ButtonLabel::ButtonLabel(QWidget *parent) :
    QWidget(parent),
    _active(false)
{
    commonInit();
}

ButtonLabel::ButtonLabel(const QString& text, QWidget* parent) :
    QWidget(parent),
    _text(text)
{
    commonInit();
}

void ButtonLabel::commonInit()
{
    _label = new QLabel(_text, this);
    _button = new QToolButton(this);

    _layout = new QHBoxLayout(this);
    _layout->addWidget(_label);
    _layout->addWidget(_button);

    setLayout(_layout);

    connect(_button, &QToolButton::clicked, this, &ButtonLabel::onButtonClicked);
}

void ButtonLabel::setText(const QString& text)
{
    _text = text;
    _label->setText(text);
}

void ButtonLabel::setIcon(const QIcon& activeIcon, const QIcon& inactiveIcon)
{
    _activeIcon = activeIcon;
    _inactiveIcon = inactiveIcon.isNull() ? activeIcon : inactiveIcon;
    _button->setIcon(activeIcon);
}

void ButtonLabel::setButtonAlignment(Qt::Alignment alignment)
{
    _buttonAlignment = alignment;
    relayout();
}

void ButtonLabel::setActive(bool active)
{
    _active = active;
    _button->setIcon(active ? _activeIcon : _inactiveIcon);
    emit activeChanged();
}

void ButtonLabel::relayout()
{
    while(_layout->count() > 0) {
        _layout->takeAt(0);
    }

    if(_buttonAlignment == Qt::AlignRight) {
        _layout->addWidget(_label);
        _layout->addWidget(_button);
    }
    else {
        _layout->addWidget(_button);
        _layout->addWidget(_label);
    }
}

void ButtonLabel::onButtonClicked()
{
    if(_active) {
        _active = false;
        _button->setIcon(_inactiveIcon);
    }
    else {
        _active = true;
        _button->setIcon(_activeIcon);
    }
    emit activeChanged();
    emit clicked();
}
