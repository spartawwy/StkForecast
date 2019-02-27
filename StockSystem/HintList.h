
#include <qdebug.h>
#include <QListWidget>

class QLineEdit;
class HintList: public QListWidget
{
    Q_OBJECT

public:
    HintList(QWidget *parent, QLineEdit *le_stock);
    ~HintList();

    void HintList::ShowUI();

    virtual void keyPressEvent(QKeyEvent *event) override;

signals:

    void choiceStr(QString);

private slots:

    void checkSlots();
     
private:

    QTimer  *myTimer;
    QLineEdit *le_stock_;
};