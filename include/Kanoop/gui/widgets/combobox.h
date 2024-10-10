#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>

class ComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ComboBox(QWidget *parent = nullptr);

    void setBold(bool bold);
    void setRowBold(int row, bool bold);

signals:

};

#endif // COMBOBOX_H
