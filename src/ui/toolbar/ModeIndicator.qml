/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


import QtQuick                              2.11
import QtQuick.Controls                     2.4

import QGroundControl                       1.0
import QGroundControl.Controls              1.0
import QGroundControl.MultiVehicleManager   1.0
import QGroundControl.ScreenTools           1.0
import QGroundControl.Palette               1.0

//-------------------------------------------------------------------------
//-- Mode Indicator
Item {
    anchors.top:    parent.top
    anchors.bottom: parent.bottom
    width:          flightModeSelector.width

    property var _flightModes:      activeVehicle ? activeVehicle.flightModes : [ ]
    property var modeParam1: 0
    property var modeParam2: 0
    property var modeParam3: 0
    property var modeParam4: 0
    property var modeParam5: 0

    on_FlightModesChanged: flightModeSelector.updateFlightModesMenu()

    QGCLabel {
        id:                     flightModeSelector
        text:                   activeVehicle ? activeVehicle.flightMode : qsTr("N/A", "No data to display")
        font.pointSize:         ScreenTools.mediumFontPointSize
        color:                  qgcPal.buttonText
        anchors.verticalCenter: parent.verticalCenter
        QGCMenu {
            id: flightModesMenu
        }
        Component {
            id: flightModeMenuItemComponent
            QGCMenuItem {
                onTriggered: activeVehicle.flightMode = text
            }
        }
        Component {
            id: parametrizedModeSelector
            Rectangle {
                id: testRect
                width: 200
                height: contentColumn.height * 1.1
                radius: ScreenTools.defaultFontPixelHeight * 0.5
                color:  qgcPal.window
                border.color:   qgcPal.text
                Column{
                    id: contentColumn
                    width: parent.width * 0.9
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.topMargin: height*0.1
                    ComboBox {
                        id: cbFlightMode
                        currentIndex: -1
                        displayText: currentIndex === -1 ? "Choose mode..." : currentText
                        model: ListModel {
                            id: cbFlightModeModel
                        }
                        width: parent.width
                        anchors.horizontalCenter: parent.horizontalCenter
                        Component.onCompleted: {
                            cbFlightModeModel.clear()
                            var i
                            for (i = 0; i < _flightModes.length; i++) {
                                cbFlightModeModel.append({'mode': _flightModes[i]})
                            }
                            currentIndex = find(activeVehicle.flightMode)
                        }
                        onCurrentIndexChanged: {
                            // Default labels
                            p1Label.text = "Param1: "
                            p2Label.text = "Param2: "
                            p3Label.text = "Param3: "
                            p4Label.text = "Param4: "
                            p5Label.text = "Param5: "

                            if (cbFlightModeModel.get(currentIndex).mode == "Load drone")
                            {
                                p1Label.text = "Payload cell: "
                                p2Label.text = "Charging cell: "
                            }
                            else if (cbFlightModeModel.get(currentIndex).mode == "Service")
                            {
                                p1Label.text = "Command id: "
                            }
                        }
                    }
                    ComboBox {
                        id: commandSelector
                        currentIndex: -1
                        displayText: currentIndex === -1 ? "Choose command..." : currentText
                        visible: cbFlightMode.currentIndex == 9
                        model: ListModel {
                            id: commandSelectorModel
                            ListElement { command: "LOCK_RELEASE" }
                            ListElement { command: "LOCK_LOCK" }
                            ListElement { command: "LOCK_STOP" }
                            ListElement { command: "OPEN_TOP_HATCH" }
                            ListElement { command: "CLOSE_TOP_HATCH" }
                            ListElement { command: "GOTO_CELL" }
                            ListElement { command: "LOAD_CHARGING_CELL" }
                            ListElement { command: "UNLOAD_CHARGING_CELL" }
                            ListElement { command: "LOAD_PAYLOAD_CELL" }
                            ListElement { command: "UNLOAD_PAYLOAD_CELL" }
                            ListElement { command: "GET_FROM_USER" }
                            ListElement { command: "UNLOAD_TO_USER" }
                            ListElement { command: "STOP" }
                            ListElement { command: "GET_PAYLOAD_FROM_DRONE" }
                            ListElement { command: "INSERT_PAYLOAD_INTO_DRONE" }
                            ListElement { command: "LOCK_PAYLOAD" }
                            ListElement { command: "RELEASE_PAYLOAD" }
                            ListElement { command: "LOCK_CHARGING_CELL_LOCK" }
                            ListElement { command: "RELEASE_CHARGING_CELL_LOCK" }
                            ListElement { command: "LOCK_TOP_HATCH_LOCK" }
                            ListElement { command: "RELEASE_TOP_HATCH_LOCK" }
                            ListElement { command: "OPEN_BOTTOM_HATCH" }
                            ListElement { command: "CLOSE_BOTTOM_HATCH" }
                            ListElement { command: "LOCK_USER_CELL_LOCK" }
                            ListElement { command: "RELEASE_USER_CELL_LOCK" }
                            ListElement { command: "GOTO_CHARGING_CELL" }
                        }
                        width: parent.width
                        anchors.horizontalCenter: parent.horizontalCenter
                        onCurrentIndexChanged: {
                            param1Box.text = currentIndex
                            p2Label.text = "Param2: "
                            p3Label.text = "Param3: "
                            p4Label.text = "Param4: "
                            p5Label.text = "Param5: "
                            if (commandSelectorModel.get(currentIndex).command == "GOTO_CELL")
                            {
                                p2Label.text = "X: "
                                p3Label.text = "Y: "
                                p4Label.text = "Z: "
                            }
                            else if (commandSelectorModel.get(currentIndex).command == "GOTO_CHARGING_CELL")
                            {
                                p2Label.text = "Xkr: "
                                p3Label.text = "Y: "
                                p4Label.text = "Z: "
                            }
                        }

                    }
                    Row {
                        width: parent.width
                        QGCLabel {
                            id: p1Label
                            text: qsTr("Param1: ")
                        }
                        TextField {
                            id: param1Box
                            text: modeParam1
                            width: parent.width * 0.5
                            anchors.verticalCenter: parent.verticalCenter

                            inputMethodHints: Qt.ImhDigitsOnly
                            validator: IntValidator {
                                bottom: 0
                                top: 32768
                            }
                            onAccepted: {
                                modeParam1 = text
                            }
                        }
                    }
                    Row {
                        width: parent.width
                        QGCLabel {
                            id: p2Label
                            text: qsTr("Param2: ")
                        }
                        TextField {
                            id: param2Box
                            text: modeParam2
                            width: parent.width * 0.5
                            anchors.verticalCenter: parent.verticalCenter
                            inputMethodHints: Qt.ImhDigitsOnly
                            validator: IntValidator {
                                bottom: 0
                                top: 32768
                            }
                            onAccepted: {
                                modeParam2 = text
                            }
                        }
                    }
                    Row {
                        width: parent.width
                        QGCLabel {
                            id: p3Label
                            text: qsTr("Param3: ")
                        }
                        TextField {
                            id: param3Box
                            text: modeParam3
                            width: parent.width * 0.5
                            anchors.verticalCenter: parent.verticalCenter
                            inputMethodHints: Qt.ImhDigitsOnly
                            validator: IntValidator {
                                bottom: 0
                                top: 32768
                            }
                            onAccepted: {
                                modeParam3 = text
                            }
                        }
                    }
                    Row {
                        width: parent.width
                        QGCLabel {
                            id: p4Label
                            text: qsTr("Param4: ")
                        }
                        TextField {
                            id: param4Box
                            text: modeParam4
                            width: parent.width * 0.5
                            anchors.verticalCenter: parent.verticalCenter
                            inputMethodHints: Qt.ImhDigitsOnly
                            validator: IntValidator {
                                bottom: 0
                                top: 32768
                            }
                            onAccepted: {
                                modeParam4 = text
                            }
                        }
                    }
                    Row {
                        width: parent.width
                        QGCLabel {
                            id: p5Label
                            text: qsTr("Param5: ")
                        }
                        TextField {
                            id: param5Box
                            text: modeParam5
                            width: parent.width * 0.5
                            anchors.verticalCenter: parent.verticalCenter
                            inputMethodHints: Qt.ImhDigitsOnly
                            validator: IntValidator {
                                bottom: 0
                                top: 32768
                            }
                            onAccepted: {
                                modeParam5 = text
                            }
                        }
                    }
                    QGCButton {
                        anchors.horizontalCenter:   parent.horizontalCenter
                        text:                       qsTr("Change mode")
                        onClicked: {
                            activeVehicle.setFlightModeParametrized(
                                        cbFlightMode.currentText,
                                        param1Box.text ? parseFloat(param1Box.text) : 0,
                                        param2Box.text ? parseFloat(param2Box.text) : 0,
                                        param3Box.text ? parseFloat(param3Box.text) : 0,
                                        param4Box.text ? parseFloat(param4Box.text) : 0,
                                        param5Box.text ? parseFloat(param5Box.text) : 0
                                        )
                        }
                    }
                }


            }

        }
        property var flightModesMenuItems: []
        function updateFlightModesMenu() {
            if (activeVehicle && activeVehicle.flightModeSetAvailable) {
                // Remove old menu items
                var i
                for (i = 0; i < flightModesMenuItems.length; i++) {
                    flightModesMenu.removeItem(flightModesMenuItems[i])
                }
                flightModesMenuItems.length = 0
                // Add new items
                for (i = 0; i < _flightModes.length; i++) {
                    var menuItem = flightModeMenuItemComponent.createObject(null, { "text": _flightModes[i] })
                    flightModesMenuItems.push(menuItem)
                    flightModesMenu.insertItem(i, menuItem)
                }
            }
        }
        Component.onCompleted: flightModeSelector.updateFlightModesMenu()
        MouseArea {
            visible:        activeVehicle && activeVehicle.flightModeSetAvailable
            anchors.fill:   parent
//            onClicked:      mainWindow.showPopUp(flightModeSelector, parametrizedModeSelector)
            onClicked: {
                if (activeVehicle.vehicleTypeName() == "Charging station")
                    mainWindow.showPopUp(flightModeSelector, parametrizedModeSelector)
                else
                    flightModesMenu.popup()
            }
        }
    }
}
