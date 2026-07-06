#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>

// 
// Dev Gothi - Help.
// A simple Help dialog explaining how to use the app. The main window opens
// this from a Help menu action (e.g. Help > How to use).
// 

class HelpDialog : public QDialog {
    Q_OBJECT
public:
    explicit HelpDialog(QWidget* parent = nullptr);
};

#endif // HELPDIALOG_H