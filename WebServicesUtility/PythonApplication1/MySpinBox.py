from PyQt5.QtWidgets import QSpinBox, QDoubleSpinBox, QStyledItemDelegate
from PyQt5.QtCore import Qt

class MySpinBox(QStyledItemDelegate):
    def __init__(self):
        super(MySpinBox, self).__init__()

    def createEditor(self, parent, option, index):
        self.spinbox = QSpinBox(parent)
        self.spinbox.setMinimum(0)
        self.spinbox.setMaximum(2000)
        self.spinbox.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        return self.spinbox

    def setEditorData(self, editor, index):
        value = int(index.model().data(index, Qt.EditRole))
        self.spinbox.setValue(value)

    def setModelData(self, editor, model, index):
        self.spinbox.interpretText()
        value = self.spinbox.value()
        model.setData(index, value, Qt.EditRole)

    def updateEditorGeometry(self, editor, option, index):
        editor.setGeometry(option.rect)

class MyDoubleSpinBox(QStyledItemDelegate):
    def __init__(self):
        super(MyDoubleSpinBox, self).__init__()

    def createEditor(self, parent, option, index):
        self.doublespinbox = QDoubleSpinBox(parent)
        self.doublespinbox.setMinimum(0.0)
        self.doublespinbox.setMaximum(2000.0)
        self.doublespinbox.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        return self.doublespinbox

    def setEditorData(self, editor, index):
        value = float(index.model().data(index, Qt.EditRole))
        self.doublespinbox.setValue(value)

    def setModelData(self, editor, model, index):
        self.doublespinbox.interpretText()
        value = self.doublespinbox.value()
        model.setData(index, value, Qt.EditRole)

    def updateEditorGeometry(self, editor, option, index):
        editor.setGeometry(option.rect)