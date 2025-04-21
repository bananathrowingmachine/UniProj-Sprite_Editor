/**
 * The dialog box that shows up when the user wants to create a new file.
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 * March 31, 2025
 *
 * Checked by [redacted], March 31, 2025
 **/

#ifndef NEWFILE_H
#define NEWFILE_H

#include <QDialog>

namespace Ui {
class NewFile;
}

class NewFile : public QDialog
{
    Q_OBJECT

public:
    explicit NewFile(QWidget *parent = nullptr);
    ~NewFile();

private:
    Ui::NewFile *ui;

private slots:

    /**
     * If okay is accepted, send the size entered to the main view and close this page.
     */
    void confirmButtonClicked();

    /**
     * If cancel is selected, close this page and do nothing.
     */
    void rejectedButton();

    /**
     * Hides the text telling the user to input a valid number.
     */
    void badNumText();

signals:

    /**
     *Sends the size entered from the user to the main view.
     * @param size - size of the canvas inputed from the user.
     */
    void sendSize(int size);
};



#endif // NEWFILE_H
