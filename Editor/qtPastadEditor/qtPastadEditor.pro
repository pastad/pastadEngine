TEMPLATE = app


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../libs/glfw_64/lib-vc2015/ -lglfw3
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../libs/glfw_64/lib-vc2015/ -lglfw3
else:unix: LIBS += -L$$PWD/../../../libs/glfw_64/lib-vc2015/ -lglfw3

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64/' -lOpenGL32
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64/' -lOpenGL32
else:unix: LIBS += -L$$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64/' -lOpenGL32

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64/' -lGlU32
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64/' -lGlU32
else:unix: LIBS += -L$$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64/' -lGlU32


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64/' -lUser32
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64/' -lUser32
else:unix: LIBS += -L$$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64/' -lUser32


INCLUDEPATH += $$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64'
DEPENDPATH += $$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64'


QT += qml quick gui opengl core
CONFIG += c++11

CONFIG += static

INCLUDEPATH += $$PWD/../../external

INCLUDEPATH += $$PWD/../../include/Effects
INCLUDEPATH += $$PWD/../../include/Model
INCLUDEPATH += $$PWD/../../include/Geometric
INCLUDEPATH += $$PWD/../../include/Render
INCLUDEPATH += $$PWD/../../include/GUI
INCLUDEPATH += $$PWD/../../include/Extension
INCLUDEPATH += $$PWD/../../include/Scene
INCLUDEPATH += $$PWD/../../include/Script
INCLUDEPATH += $$PWD/../../include/Shaders
INCLUDEPATH += $$PWD/../../include/Subsystems
INCLUDEPATH += $$PWD/../../include/Structures
INCLUDEPATH += $$PWD/../../include

INCLUDEPATH += $$PWD/../../pastadEditor

INCLUDEPATH += C:\Users\pastad\Documents\libs\glm
INCLUDEPATH += C:\Users\pastad\Documents\libs\glfw_64\include
INCLUDEPATH += C:\Users\pastad\Documents\libs\assimp\include
INCLUDEPATH += C:\Users\pastad\Documents\libs\tinyxml2-master
INCLUDEPATH += C:\Users\pastad\Documents\libs\soil/src
INCLUDEPATH += C:\Users\pastad\Documents\libs\SFML-2.4.0\include
INCLUDEPATH += C:\Users\pastad\Documents\libs\boost_1_59_0
INCLUDEPATH += C:\Users\pastad\Documents\libs\freetype-2.6.5\include



SOURCES += main.cpp \
    openglwindow.cpp \
    ../../src/Effects/Water.cpp \
    ../../src/Effects/Wave.cpp \
    ../../src/Extension/Skybox.cpp \
    ../../src/Extension/Terrain.cpp \
    ../../src/Geometric/BoundingBox.cpp \
    ../../src/Geometric/Plane.cpp \
    ../../src/Geometric/Ray.cpp \
    ../../src/GUI/Button.cpp \
    ../../src/GUI/EditText.cpp \
    ../../src/GUI/EngineGUI.cpp \
    ../../src/GUI/FileViewer.cpp \
    ../../src/GUI/GUI.cpp \
    ../../src/GUI/Image.cpp \
    ../../src/GUI/Text.cpp \
    ../../src/Model/AnimationMesh.cpp \
    ../../src/Model/IndexedRepresentation.cpp \
    ../../src/Model/Material.cpp \
    ../../src/Model/Mesh.cpp \
    ../../src/Model/Model.cpp \
    ../../src/Model/Object.cpp \
    ../../src/Model/Texture.cpp \
    ../../src/Render/DirectionalShadowBuffer.cpp \
    ../../src/Render/GBuffer.cpp \
    ../../src/Render/JitterTexture.cpp \
    ../../src/Render/NoiseTexture.cpp \
    ../../src/Render/PointShadowBuffer.cpp \
    ../../src/Render/Quad.cpp \
    ../../src/Render/RenderBuffer.cpp \
    ../../src/Render/TransparentBuffer.cpp \
    ../../src/Scene/Scene.cpp \
    ../../src/Scene/SceneTreeElement.cpp \
    ../../src/Script/ExternalTriggerScriptElement.cpp \
    ../../src/Script/RotationScriptElement.cpp \
    ../../src/Script/Script.cpp \
    ../../src/Script/ScriptElement.cpp \
    ../../src/Script/TranslationScriptElement.cpp \
    ../../src/Shaders/ImageShader.cpp \
    ../../src/Shaders/PostProcessingShader.cpp \
    ../../src/Shaders/RenderBaseShader.cpp \
    ../../src/Shaders/RenderShader.cpp \
    ../../src/Shaders/Shader.cpp \
    ../../src/Shaders/ShadowShader.cpp \
    ../../src/Shaders/SkyboxShader.cpp \
    ../../src/Shaders/TextShader.cpp \
    ../../src/Structures/Camera.cpp \
    ../../src/Structures/Light.cpp \
    ../../src/Structures/Transform.cpp \
    ../../src/Subsystems/IOSubsystem.cpp \
    ../../src/Subsystems/PhysicSubsystem.cpp \
    ../../src/Subsystems/RenderSubsystem.cpp \
    ../../src/Subsystems/Subsystem.cpp \
    ../../src/Engine.cpp \
    ../../src/Helper.cpp \
    ../../src/Log.cpp \
    ../../src/RessourceManager.cpp \
    ../../external/gl_core_4_.cpp \
    editorwindow.cpp \
    ObjectDatabase.cpp \
    PastadEditorObjectDatabase.cpp \
    PastadEditorSuccessorSelector.cpp


RESOURCES += qml.qrc



# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_STATIC_BUILD

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    openglwindow.h \
    ../../include/Effects/Water.h \
    ../../include/Effects/Wave.h \
    ../../include/Extension/Skybox.h \
    ../../include/Extension/Terrain.h \
    ../../include/Geometric/BoundingBox.h \
    ../../include/Geometric/Plane.h \
    ../../include/Geometric/Ray.h \
    ../../include/GUI/Button.h \
    ../../include/GUI/EditText.h \
    ../../include/GUI/EngineGUI.h \
    ../../include/GUI/FileViewer.h \
    ../../include/GUI/GUI.h \
    ../../include/GUI/Image.h \
    ../../include/GUI/Text.h \
    ../../include/Model/AnimationMesh.h \
    ../../include/Model/IndexedRepresentation.h \
    ../../include/Model/Material.h \
    ../../include/Model/Mesh.h \
    ../../include/Model/Model.h \
    ../../include/Model/Object.h \
    ../../include/Model/Texture.h \
    ../../include/Render/DirectionalShadowBuffer.h \
    ../../include/Render/GBuffer.h \
    ../../include/Render/JitterTexture.h \
    ../../include/Render/NoiseTexture.h \
    ../../include/Render/PointShadowBuffer.h \
    ../../include/Render/Quad.h \
    ../../include/Render/RenderBuffer.h \
    ../../include/Render/TransparentBuffer.h \
    ../../include/Scene/Scene.h \
    ../../include/Scene/SceneTreeElement.h \
    ../../include/Script/ExternalTriggerScriptElement.h \
    ../../include/Script/RotationScriptElement.h \
    ../../include/Script/Script.h \
    ../../include/Script/ScriptElement.h \
    ../../include/Script/TranslationScriptElement.h \
    ../../include/Shaders/ImageShader.h \
    ../../include/Shaders/PostProcessingShader.h \
    ../../include/Shaders/RenderBaseShader.h \
    ../../include/Shaders/RenderShader.h \
    ../../include/Shaders/Shader.h \
    ../../include/Shaders/ShadowShader.h \
    ../../include/Shaders/SkyboxShader.h \
    ../../include/Shaders/TextShader.h \
    ../../include/Structures/Camera.h \
    ../../include/Structures/Light.h \
    ../../include/Structures/Transform.h \
    ../../include/Subsystems/IOSubsystem.h \
    ../../include/Subsystems/PhysicSubsystem.h \
    ../../include/Subsystems/RenderSubsystem.h \
    ../../include/Subsystems/Subsystem.h \
    ../../include/Engine.h \
    ../../include/Helper.h \
    ../../include/Log.h \
    ../../include/RessourceManager.h \
    ../../include/ui_editor.h \
    ../../include/ui_mainwindow.h \
    ../../external/gl_core_4_.hpp \
    editorwindow.h \
    ObjectDatabase.h \
    PastadEditorObjectDatabase.h \
    ui_ObjectDatabaseEditor.h \
    PastadEditorSuccessorSelector.h




INCLUDEPATH += $$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64'
DEPENDPATH += $$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64'




INCLUDEPATH += $$PWD/../../../libs/glfw_64/lib-vc2015
DEPENDPATH += $$PWD/../../../libs/glfw_64/lib-vc2015







win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../libs/boost_1_59_0/lib64-msvc-14.0/ -llibboost_filesystem-vc140-mt-1_59
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../libs/boost_1_59_0/lib64-msvc-14.0/ -llibboost_filesystem-vc140-mt-gd-1_59
else:unix: LIBS += -L$$PWD/../../../libs/boost_1_59_0/lib64-msvc-14.0/ -llibboost_filesystem-vc140-mt-1_59

INCLUDEPATH += $$PWD/../../../libs/boost_1_59_0/lib64-msvc-14.0
DEPENDPATH += $$PWD/../../../libs/boost_1_59_0/lib64-msvc-14.0

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../libs/tinyxml2-master/tinyxml2/bin/x64-Release-Dll/ -ltinyxml2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../libs/tinyxml2-master/tinyxml2/bin/x64-Release-Dll/ -ltinyxml2
else:unix: LIBS += -L$$PWD/../../../libs/tinyxml2-master/tinyxml2/bin/x64-Release-Dll/ -ltinyxml2

INCLUDEPATH += $$PWD/../../../libs/tinyxml2-master/tinyxml2/bin/x64-Release-Dll
DEPENDPATH += $$PWD/../../../libs/tinyxml2-master/tinyxml2/bin/x64-Release-Dll

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../libs/assimp/lib64/ -lassimp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../libs/assimp/lib64/ -lassimp
else:unix: LIBS += -L$$PWD/../../../libs/assimp/lib64/ -lassimp

INCLUDEPATH += $$PWD/../../../libs/assimp/lib64
DEPENDPATH += $$PWD/../../../libs/assimp/lib64

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../libs/soil/lib/ -lSOIL_64
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../libs/soil/lib/ -lSOIL_64
else:unix: LIBS += -L$$PWD/../../../libs/soil/lib/ -lSOIL_64

INCLUDEPATH += $$PWD/../../../libs/soil
DEPENDPATH += $$PWD/../../../libs/soil

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../libs/freetype-2.6.2/objs/vc2010/x64/ -lfreetype262MT
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../libs/freetype-2.6.2/objs/vc2010/x64/ -lfreetype262MT
else:unix: LIBS += -L$$PWD/../../../libs/freetype-2.6.2/objs/vc2010/x64/ -lfreetype262MT

INCLUDEPATH += $$PWD/../../../libs/freetype-2.6.2/objs/vc2010/x64
DEPENDPATH += $$PWD/../../../libs/freetype-2.6.2/objs/vc2010/x64






INCLUDEPATH += $$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64'
DEPENDPATH += $$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64'

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64/' -lGdi32
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64/' -lGdi32
else:unix: LIBS += -L$$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64/' -lGdi32

INCLUDEPATH += $$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64'
DEPENDPATH += $$PWD/'../../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64'

FORMS += \
    editorwindow.ui \
    ObjectDatabaseEditor.ui \
    scriptwindow \
    SuccessorSelector.ui
