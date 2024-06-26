#ifndef TOP_H
#define TOP_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class TOP;
}

class TOP : public QWidget
{
    Q_OBJECT

public:
    explicit TOP(QWidget *parent = nullptr);
    ~TOP();

private:
    Ui::TOP *ui;
private slots:
    void slot_select_rote();
    void onItemClicked(QListWidgetItem* item);
};

#endif // TOP_H
