#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "EcologyCore/ecologycore.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();
    void update();

private:
    Ui::Form *ui;
};

#endif // FORM_H
