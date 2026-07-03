import QtQuick
import QtQuick.Controls
import QtQuick.Effects


Item{
    id:root

    function rgb(r, g, b) {
        return Qt.rgba(r / 255, g / 255, b / 255, 1)
    }

    property int status:0
    property int answer_status:0

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
                    visible:root.status == 1 && root.answer_status==0
                    Column{
                        spacing:10
                        anchors.fill: parent
                        anchors.margins: 20
                        Text {
                            color:"white"
                            anchors.left: parent.left
                            text: "問題No." + String(dq.no) + "　　" + dq.kind

                        }

                        Text {
                            color:"white"
                            anchors.left: parent.left
                            text: dq.question
                        }

                        Text {
                            color:"white"
                            width:parent.width
                            anchors.left: parent.left
                            text: dq.q_sentence
                            wrapMode: Text.WordWrap
                        }

                        Button{
                            text:"答えを見る"
                            onClicked:root.answer_status=1
                        }
                    }
                }

                Item{
                    visible:root.status == 1 && root.answer_status==1
                    Column{
                        spacing:10
                        anchors.fill: parent
                        anchors.margins: 20
                        Text {
                            color:"white"
                            anchors.left: parent.left
                            text: "問題No." + String(dq.no) + "　　" + dq.kind

                        }

                        Text {
                            color:"white"
                            anchors.left: parent.left
                            text: dq.question
                        }

                        Text {
                            color:"white"
                            width:parent.width
                            anchors.left: parent.left
                            text: dq.q_sentence
                            wrapMode: Text.WordWrap
                        }

                        Button{
                            text:"正解"
                            onClicked:{
                                root.answer_status = 0;
                                mainWindow.questionChangeClicked(1)
                            }
                        }

                        Button{
                            text:"間違えた"
                            onClicked:{
                                root.answer_status = 0;
                                mainWindow.questionChangeClicked(0)
                            }
                        }
                    }
                }

                Item{
                    visible:root.status==0
                    Text{
                        color:"white"
                        width:parent.width
                        anchors.left: parent.left
                        text:"ファイルを読み込んで出題する問題をチェックして下さい"
                    }
                }


            }
        }
    }
}