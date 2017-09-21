#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QMessageBox>

#define MSGBOX_WARNING(parent, msg) QMessageBox::warning(parent, tr("order"), tr(msg));

#endif // MESSAGEBOX_H