import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Effects
import QtMultimedia



Item{
    id:root

    function rgb(r, g, b) {
        return Qt.rgba(r / 255, g / 255, b / 255, 1)
    }

    property int status:0
    property int answer_status:0
    property bool soundon:true


    SoundEffect {
        id: correctSound
        source: "qrc:/sound/sound/correct.wav"
    }

    SoundEffect {
        id: wrongSound
        source: "qrc:/sound/sound/wrong.wav"
    }

    SoundEffect {
        id: questSound
        source: "qrc:/sound/sound/quest.wav"
    }

    Item {
        id:content


        property int overlap: 30
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 30
        anchors.rightMargin: 30
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        Image {
           id: img
            source: "qrc:/image/image/pause_chara2.png"

            width: 155
            height: 115
            fillMode: Image.PreserveAspectFit

            x: 0
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -1
            z: 1

            layer.enabled: true
            layer.effect: MultiEffect {
                shadowEnabled: true
                shadowColor: Qt.rgba(0, 0, 0, 0.35)
                shadowBlur: 0.6
                shadowHorizontalOffset: 6
                shadowVerticalOffset: 6
                blurMax: 32
            }
        }

        RectangularShadow {
            anchors.fill: boxFrame
            radius: boxFrame.radius
            blur: 24
            spread: 2
            offset.x: 6
            offset.y: 6
            color: Qt.rgba(0, 0, 0, 0.35)
        }

        Rectangle {
            id: boxFrame

            anchors.left: img.right
            anchors.leftMargin: -content.overlap
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -10

            height: 230
            radius: 16

            property int bw: 10

            gradient: Gradient {
                //orientation: Gradient.Horizontal   // 横グラデ。縦なら消してOK
                GradientStop { position: 0.0; color: rgb(196,155,119) }
                GradientStop { position: 1.0; color: rgb(112,87,66) }
            }

            Rectangle {
                id: innerBox
                anchors.fill: parent
                anchors.margins: boxFrame.bw
                clip:true

                property int cellSize: 30
                radius: Math.max(0, boxFrame.radius - boxFrame.bw)
                color: rgb(32,49,32)

                Repeater {
                    model: Math.max(0,Math.floor(innerBox.width / innerBox.cellSize)) + 1

                    Rectangle {
                        width: 2
                        height: innerBox.height

                        x: index * innerBox.cellSize
                        y: 0

                        color: rgb(27,39,27)
                    }
                }

                Repeater {
                    model: Math.max(0,Math.floor(innerBox.height / innerBox.cellSize)) + 1

                    Rectangle {
                        width: innerBox.width
                        height: 2

                        x: 0
                        y: index * innerBox.cellSize

                        color: rgb(27,39,27)
                    }
                }


                Rectangle {
                    width: innerBox.width
                    height: 7

                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: -3

                    color: rgb(211,181,154)
                }

                Item{
                    id:questionPage
                    property bool active: root.status == 1 && root.answer_status == 0

                    visible: opacity > 0.01
                    opacity: active ? 1.0 : 0.0

                    Behavior on opacity {
                        NumberAnimation {
                            duration: 200
                            easing.type: Easing.OutQuad
                        }
                    }

                    anchors.fill:parent
                    Text {
                        color:"white"
                        anchors.left: parent.left
                        anchors.leftMargin: 12
                        anchors.top:parent.top
                        anchors.topMargin: 12
                        font.pixelSize: 14
                        text: "問題No." + String(dq.no) + "　　" + dq.kind

                    }

                    Column{
                        spacing:6
                        anchors.fill: parent
                        anchors.margins: 16

                        Text {
                            color:"white"
                            width:parent.width
                            font.pixelSize: 32
                            text: dq.question
                            horizontalAlignment: Text.AlignHCenter
                        }

                        Text {
                            color:"white"
                            font.pixelSize: 24
                            width:parent.width
                            text: dq.q_sentence
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }

                    Button{
                        id:answerButton
                        text:" 答えを見る(Space) "

                        font.pixelSize: 16
                        onClicked:{
                            root.answer_status=1
                            if(root.soundon){
                                questSound.play()
                            }
                        }
                        anchors.right: parent.right
                        anchors.rightMargin: 12
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 12

                        background: Rectangle {
                            radius: 8
                            color: answerButton.hovered ? "white" : "#ddd"
                            border.color: "#ffffff"
                            border.width: 1

                            Behavior on color {
                                ColorAnimation {
                                    duration: 150
                                    easing.type: Easing.OutQuad
                                }
                            }
                        }
                    }

                    Shortcut {
                        enabled:questionPage.active
                        sequence: "Space"
                        onActivated: {
                            root.answer_status=1
                            if(root.soundon){
                                questSound.play()
                            }
                        }
                    }
                }

                Item{
                    id:answerPage
                    property bool active: root.status == 1 && root.answer_status == 1

                    visible: active
                    opacity: active ? 1.0 : 0.0

                    Behavior on opacity {
                        NumberAnimation {
                            duration: 200
                            easing.type: Easing.OutQuad
                        }
                    }

                    anchors.fill:parent

                    Text {
                        color:"white"
                        anchors.left: parent.left
                        anchors.leftMargin: 12
                        anchors.top:parent.top
                        anchors.topMargin: 12
                        font.pixelSize: 14
                        text: "問題No." + String(dq.no) + "　　" + dq.kind

                    }

                    Column{
                        spacing:6
                        anchors.fill: parent
                        anchors.margins: 16

                        Text {
                            width:parent.width
                            font.pixelSize: 32
                            text: dq.question + " ： " + dq.answer
                            color:"#aae"
                            horizontalAlignment: Text.AlignHCenter
                        }

                        Text {
                            color:"white"
                            font.pixelSize: 24
                            width:parent.width
                            text: dq.q_sentence
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignHCenter
                        }

                        Text {
                            color:"white"
                            font.pixelSize: 20
                            width:parent.width
                            text: dq.a_sentence
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignHCenter
                        }

                    }
                    Row{
                        spacing:6
                        anchors.right: parent.right
                        anchors.rightMargin: 12
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 12

                        Button{
                            id:correctButton
                            text:" 正解(Z) "
                            font.pixelSize: 16

                            background: Rectangle {
                                radius: 8
                                color: correctButton.hovered? "white" : "#99f"
                                border.color: "#ffffff"
                                border.width: 1

                                Behavior on color {
                                    ColorAnimation {
                                        duration: 150
                                        easing.type: Easing.OutQuad
                                    }
                                }
                            }
                            onClicked:{
                                root.answer_status = 0;
                                mainWindow.questionChangeClicked(1)
                                if(root.soundon){
                                    correctSound.play()
                                }
                            }
                        }

                        Button{
                            id:wrongButton
                            text:" 誤答(X) "
                            font.pixelSize: 16
                            background: Rectangle {
                                radius: 8
                                color: wrongButton.hovered ? "white" : "#f99"
                                border.color: "#ffffff"
                                border.width: 1

                                Behavior on color {
                                    ColorAnimation {
                                        duration: 150
                                        easing.type: Easing.OutQuad
                                    }
                                }
                            }
                            onClicked:{
                                root.answer_status = 0
                                mainWindow.questionChangeClicked(0)
                                if(root.soundon){
                                    wrongSound.play()
                                }
                            }
                        }

                        Shortcut {
                            sequence: "Z"
                            enabled:answerPage.active
                            onActivated: {
                                root.answer_status = 0
                                mainWindow.questionChangeClicked(1)
                                if(root.soundon){
                                    correctSound.play()
                                }
                            }
                        }

                        Shortcut {
                            sequence: "X"
                            enabled:answerPage.active
                            onActivated: {
                                root.answer_status = 0
                                mainWindow.questionChangeClicked(0)
                                if(root.soundon){
                                    wrongSound.play()
                                }
                            }
                        }
                    }
                }

                Item{
                    id:waitingPage
                    property bool active: root.status == 0

                    visible: opacity > 0.01
                    opacity: active ? 1.0 : 0.0

                    Behavior on opacity {
                        NumberAnimation {
                            duration: 200
                            easing.type: Easing.OutQuad
                        }
                    }

                    anchors.fill:parent

                    Text{
                        color:"white"
                        width:parent.width
                        anchors.centerIn: parent
                        font.pixelSize: 28
                        text:"ファイルを読み込んで出題する問題をチェックして下さい"

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                    }
                }


            }
        }
    }
}