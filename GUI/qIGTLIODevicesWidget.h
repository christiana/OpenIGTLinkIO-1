#ifndef QIGTLIODEVICESWIDGET_H
#define QIGTLIODEVICESWIDGET_H

#include <QWidget>

// CTK includes
#include <ctkVTKObject.h>

// igtlio includes
#include "igtlioGUIExport.h"
class qIGTLIODevicesModel;
class QVBoxLayout;
class qIGTLIOConnectorPropertyWidget;
class QItemSelectionModel;

class QTreeView;
#include <vtkSmartPointer.h>
typedef vtkSmartPointer<class vtkIGTLIOLogic> vtkIGTLIOLogicPointer;

class OPENIGTLINKIO_GUI_EXPORT qIGTLIODevicesWidget : public QWidget
{
  Q_OBJECT
  QVTK_OBJECT
public:
  qIGTLIODevicesWidget();
  void setLogic(vtkIGTLIOLogicPointer logic);

signals:

private slots:
  void onConnectionsChanged(vtkObject *caller, void *connector, unsigned long event, void *b);
  void onCurrentConnectorChanged();
private:
  vtkIGTLIOLogicPointer Logic;
  qIGTLIODevicesModel* DevicesModel;
  QItemSelectionModel* SelectionModel;
  QTreeView* DevicesListView;

};

#endif // QIGTLIODEVICESWIDGET_H
