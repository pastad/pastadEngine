/****************************************************************************
** Meta object code from reading C++ file 'PastadEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../PastadEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PastadEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PastadEditor_t {
    QByteArrayData data[77];
    char stringdata0[1955];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PastadEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PastadEditor_t qt_meta_stringdata_PastadEditor = {
    {
QT_MOC_LITERAL(0, 0, 12), // "PastadEditor"
QT_MOC_LITERAL(1, 13, 28), // "on_actionLoadScene_triggered"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 28), // "on_actionSaveScene_triggered"
QT_MOC_LITERAL(4, 72, 36), // "on_actionNewObjectDatabase_tr..."
QT_MOC_LITERAL(5, 109, 37), // "on_actionLoadObjectDatabase_t..."
QT_MOC_LITERAL(6, 147, 37), // "on_actionEditObjectDatabase_t..."
QT_MOC_LITERAL(7, 185, 24), // "on_pb_scene_play_clicked"
QT_MOC_LITERAL(8, 210, 25), // "on_pb_scene_pause_clicked"
QT_MOC_LITERAL(9, 236, 25), // "on_pb_scene_reset_clicked"
QT_MOC_LITERAL(10, 262, 29), // "on_pb_refresh_shaders_clicked"
QT_MOC_LITERAL(11, 292, 7), // "setTime"
QT_MOC_LITERAL(12, 300, 11), // "std::string"
QT_MOC_LITERAL(13, 312, 4), // "time"
QT_MOC_LITERAL(14, 317, 7), // "savePos"
QT_MOC_LITERAL(15, 325, 7), // "saveRot"
QT_MOC_LITERAL(16, 333, 15), // "saveObjectSpecs"
QT_MOC_LITERAL(17, 349, 15), // "refreshSelected"
QT_MOC_LITERAL(18, 365, 17), // "refreshObjectList"
QT_MOC_LITERAL(19, 383, 21), // "refreshObjectDatabase"
QT_MOC_LITERAL(20, 405, 25), // "on_pb_objects_add_clicked"
QT_MOC_LITERAL(21, 431, 27), // "on_pb_object_remove_clicked"
QT_MOC_LITERAL(22, 459, 38), // "on_lw_objects_active_currentR..."
QT_MOC_LITERAL(23, 498, 3), // "row"
QT_MOC_LITERAL(24, 502, 39), // "on_lw_objects_chooser_current..."
QT_MOC_LITERAL(25, 542, 31), // "on_le_obj_pos_x_editingFinished"
QT_MOC_LITERAL(26, 574, 31), // "on_le_obj_pos_y_editingFinished"
QT_MOC_LITERAL(27, 606, 31), // "on_le_obj_pos_z_editingFinished"
QT_MOC_LITERAL(28, 638, 31), // "on_le_obj_rot_x_editingFinished"
QT_MOC_LITERAL(29, 670, 31), // "on_le_obj_rot_y_editingFinished"
QT_MOC_LITERAL(30, 702, 31), // "on_le_obj_rot_z_editingFinished"
QT_MOC_LITERAL(31, 734, 23), // "on_rb_obj_fixed_clicked"
QT_MOC_LITERAL(32, 758, 5), // "value"
QT_MOC_LITERAL(33, 764, 30), // "on_rb_obj_apply_physic_clicked"
QT_MOC_LITERAL(34, 795, 31), // "on_rb_obj_physic_static_clicked"
QT_MOC_LITERAL(35, 827, 25), // "on_rb_obj_visible_clicked"
QT_MOC_LITERAL(36, 853, 29), // "on_rb_obj_shadow_only_clicked"
QT_MOC_LITERAL(37, 883, 12), // "saveLightPos"
QT_MOC_LITERAL(38, 896, 12), // "saveLightRot"
QT_MOC_LITERAL(39, 909, 16), // "refreshLightList"
QT_MOC_LITERAL(40, 926, 24), // "on_pb_lights_add_clicked"
QT_MOC_LITERAL(41, 951, 26), // "on_pb_light_remove_clicked"
QT_MOC_LITERAL(42, 978, 37), // "on_lw_lights_active_currentRo..."
QT_MOC_LITERAL(43, 1016, 33), // "on_le_light_pos_x_editingFini..."
QT_MOC_LITERAL(44, 1050, 33), // "on_le_light_pos_y_editingFini..."
QT_MOC_LITERAL(45, 1084, 33), // "on_le_light_pos_z_editingFini..."
QT_MOC_LITERAL(46, 1118, 33), // "on_le_light_rot_x_editingFini..."
QT_MOC_LITERAL(47, 1152, 33), // "on_le_light_rot_y_editingFini..."
QT_MOC_LITERAL(48, 1186, 33), // "on_le_light_rot_z_editingFini..."
QT_MOC_LITERAL(49, 1220, 26), // "on_sb_light_r_valueChanged"
QT_MOC_LITERAL(50, 1247, 1), // "i"
QT_MOC_LITERAL(51, 1249, 26), // "on_sb_light_g_valueChanged"
QT_MOC_LITERAL(52, 1276, 26), // "on_sb_light_b_valueChanged"
QT_MOC_LITERAL(53, 1303, 34), // "on_sb_light_intensity_valueCh..."
QT_MOC_LITERAL(54, 1338, 1), // "d"
QT_MOC_LITERAL(55, 1340, 14), // "refreshScripts"
QT_MOC_LITERAL(56, 1355, 20), // "refreshScriptElement"
QT_MOC_LITERAL(57, 1376, 24), // "on_pb_add_script_clicked"
QT_MOC_LITERAL(58, 1401, 27), // "on_pb_remove_script_clicked"
QT_MOC_LITERAL(59, 1429, 27), // "on_pb_add_successor_clicked"
QT_MOC_LITERAL(60, 1457, 34), // "on_lw_sc_objects_currentRowCh..."
QT_MOC_LITERAL(61, 1492, 42), // "on_lw_sc_script_elements_curr..."
QT_MOC_LITERAL(62, 1535, 29), // "on_le_sc_val1_editingFinished"
QT_MOC_LITERAL(63, 1565, 29), // "on_le_sc_val2_editingFinished"
QT_MOC_LITERAL(64, 1595, 29), // "on_le_sc_val3_editingFinished"
QT_MOC_LITERAL(65, 1625, 29), // "on_le_sc_from_editingFinished"
QT_MOC_LITERAL(66, 1655, 27), // "on_le_sc_to_editingFinished"
QT_MOC_LITERAL(67, 1683, 33), // "on_le_sc_duration_editingFini..."
QT_MOC_LITERAL(68, 1717, 17), // "saveScriptElement"
QT_MOC_LITERAL(69, 1735, 36), // "on_cb_sc_trigger_currentIndex..."
QT_MOC_LITERAL(70, 1772, 3), // "idx"
QT_MOC_LITERAL(71, 1776, 37), // "on_cb_shadow_type_currentInde..."
QT_MOC_LITERAL(72, 1814, 43), // "on_cb_shadow_type_point_curre..."
QT_MOC_LITERAL(73, 1858, 39), // "on_cb_anti_aliasing_currentIn..."
QT_MOC_LITERAL(74, 1898, 17), // "on_rb_hdr_clicked"
QT_MOC_LITERAL(75, 1916, 19), // "on_rb_bloom_clicked"
QT_MOC_LITERAL(76, 1936, 18) // "on_rb_ssao_clicked"

    },
    "PastadEditor\0on_actionLoadScene_triggered\0"
    "\0on_actionSaveScene_triggered\0"
    "on_actionNewObjectDatabase_triggered\0"
    "on_actionLoadObjectDatabase_triggered\0"
    "on_actionEditObjectDatabase_triggered\0"
    "on_pb_scene_play_clicked\0"
    "on_pb_scene_pause_clicked\0"
    "on_pb_scene_reset_clicked\0"
    "on_pb_refresh_shaders_clicked\0setTime\0"
    "std::string\0time\0savePos\0saveRot\0"
    "saveObjectSpecs\0refreshSelected\0"
    "refreshObjectList\0refreshObjectDatabase\0"
    "on_pb_objects_add_clicked\0"
    "on_pb_object_remove_clicked\0"
    "on_lw_objects_active_currentRowChanged\0"
    "row\0on_lw_objects_chooser_currentRowChanged\0"
    "on_le_obj_pos_x_editingFinished\0"
    "on_le_obj_pos_y_editingFinished\0"
    "on_le_obj_pos_z_editingFinished\0"
    "on_le_obj_rot_x_editingFinished\0"
    "on_le_obj_rot_y_editingFinished\0"
    "on_le_obj_rot_z_editingFinished\0"
    "on_rb_obj_fixed_clicked\0value\0"
    "on_rb_obj_apply_physic_clicked\0"
    "on_rb_obj_physic_static_clicked\0"
    "on_rb_obj_visible_clicked\0"
    "on_rb_obj_shadow_only_clicked\0"
    "saveLightPos\0saveLightRot\0refreshLightList\0"
    "on_pb_lights_add_clicked\0"
    "on_pb_light_remove_clicked\0"
    "on_lw_lights_active_currentRowChanged\0"
    "on_le_light_pos_x_editingFinished\0"
    "on_le_light_pos_y_editingFinished\0"
    "on_le_light_pos_z_editingFinished\0"
    "on_le_light_rot_x_editingFinished\0"
    "on_le_light_rot_y_editingFinished\0"
    "on_le_light_rot_z_editingFinished\0"
    "on_sb_light_r_valueChanged\0i\0"
    "on_sb_light_g_valueChanged\0"
    "on_sb_light_b_valueChanged\0"
    "on_sb_light_intensity_valueChanged\0d\0"
    "refreshScripts\0refreshScriptElement\0"
    "on_pb_add_script_clicked\0"
    "on_pb_remove_script_clicked\0"
    "on_pb_add_successor_clicked\0"
    "on_lw_sc_objects_currentRowChanged\0"
    "on_lw_sc_script_elements_currentRowChanged\0"
    "on_le_sc_val1_editingFinished\0"
    "on_le_sc_val2_editingFinished\0"
    "on_le_sc_val3_editingFinished\0"
    "on_le_sc_from_editingFinished\0"
    "on_le_sc_to_editingFinished\0"
    "on_le_sc_duration_editingFinished\0"
    "saveScriptElement\0"
    "on_cb_sc_trigger_currentIndexChanged\0"
    "idx\0on_cb_shadow_type_currentIndexChanged\0"
    "on_cb_shadow_type_point_currentIndexChanged\0"
    "on_cb_anti_aliasing_currentIndexChanged\0"
    "on_rb_hdr_clicked\0on_rb_bloom_clicked\0"
    "on_rb_ssao_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PastadEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      68,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  354,    2, 0x0a /* Public */,
       3,    0,  355,    2, 0x0a /* Public */,
       4,    0,  356,    2, 0x0a /* Public */,
       5,    0,  357,    2, 0x0a /* Public */,
       6,    0,  358,    2, 0x0a /* Public */,
       7,    0,  359,    2, 0x0a /* Public */,
       8,    0,  360,    2, 0x0a /* Public */,
       9,    0,  361,    2, 0x0a /* Public */,
      10,    0,  362,    2, 0x0a /* Public */,
      11,    1,  363,    2, 0x0a /* Public */,
      14,    0,  366,    2, 0x0a /* Public */,
      15,    0,  367,    2, 0x0a /* Public */,
      16,    0,  368,    2, 0x0a /* Public */,
      17,    0,  369,    2, 0x0a /* Public */,
      18,    0,  370,    2, 0x0a /* Public */,
      19,    0,  371,    2, 0x0a /* Public */,
      20,    0,  372,    2, 0x0a /* Public */,
      21,    0,  373,    2, 0x0a /* Public */,
      22,    1,  374,    2, 0x0a /* Public */,
      24,    1,  377,    2, 0x0a /* Public */,
      25,    0,  380,    2, 0x0a /* Public */,
      26,    0,  381,    2, 0x0a /* Public */,
      27,    0,  382,    2, 0x0a /* Public */,
      28,    0,  383,    2, 0x0a /* Public */,
      29,    0,  384,    2, 0x0a /* Public */,
      30,    0,  385,    2, 0x0a /* Public */,
      31,    1,  386,    2, 0x0a /* Public */,
      33,    1,  389,    2, 0x0a /* Public */,
      34,    1,  392,    2, 0x0a /* Public */,
      35,    1,  395,    2, 0x0a /* Public */,
      36,    1,  398,    2, 0x0a /* Public */,
      37,    0,  401,    2, 0x0a /* Public */,
      38,    0,  402,    2, 0x0a /* Public */,
      39,    0,  403,    2, 0x0a /* Public */,
      40,    0,  404,    2, 0x0a /* Public */,
      41,    0,  405,    2, 0x0a /* Public */,
      42,    1,  406,    2, 0x0a /* Public */,
      43,    0,  409,    2, 0x0a /* Public */,
      44,    0,  410,    2, 0x0a /* Public */,
      45,    0,  411,    2, 0x0a /* Public */,
      46,    0,  412,    2, 0x0a /* Public */,
      47,    0,  413,    2, 0x0a /* Public */,
      48,    0,  414,    2, 0x0a /* Public */,
      49,    1,  415,    2, 0x0a /* Public */,
      51,    1,  418,    2, 0x0a /* Public */,
      52,    1,  421,    2, 0x0a /* Public */,
      53,    1,  424,    2, 0x0a /* Public */,
      55,    0,  427,    2, 0x0a /* Public */,
      56,    0,  428,    2, 0x0a /* Public */,
      57,    0,  429,    2, 0x0a /* Public */,
      58,    0,  430,    2, 0x0a /* Public */,
      59,    0,  431,    2, 0x0a /* Public */,
      60,    1,  432,    2, 0x0a /* Public */,
      61,    1,  435,    2, 0x0a /* Public */,
      62,    0,  438,    2, 0x0a /* Public */,
      63,    0,  439,    2, 0x0a /* Public */,
      64,    0,  440,    2, 0x0a /* Public */,
      65,    0,  441,    2, 0x0a /* Public */,
      66,    0,  442,    2, 0x0a /* Public */,
      67,    0,  443,    2, 0x0a /* Public */,
      68,    0,  444,    2, 0x0a /* Public */,
      69,    1,  445,    2, 0x0a /* Public */,
      71,    1,  448,    2, 0x0a /* Public */,
      72,    1,  451,    2, 0x0a /* Public */,
      73,    1,  454,    2, 0x0a /* Public */,
      74,    1,  457,    2, 0x0a /* Public */,
      75,    1,  460,    2, 0x0a /* Public */,
      76,    1,  463,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   32,
    QMetaType::Void, QMetaType::Bool,   32,
    QMetaType::Void, QMetaType::Bool,   32,
    QMetaType::Void, QMetaType::Bool,   32,
    QMetaType::Void, QMetaType::Bool,   32,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   50,
    QMetaType::Void, QMetaType::Int,   50,
    QMetaType::Void, QMetaType::Int,   50,
    QMetaType::Void, QMetaType::Double,   54,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   70,
    QMetaType::Void, QMetaType::Int,   70,
    QMetaType::Void, QMetaType::Int,   70,
    QMetaType::Void, QMetaType::Int,   70,
    QMetaType::Void, QMetaType::Bool,   32,
    QMetaType::Void, QMetaType::Bool,   32,
    QMetaType::Void, QMetaType::Bool,   32,

       0        // eod
};

void PastadEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PastadEditor *_t = static_cast<PastadEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_actionLoadScene_triggered(); break;
        case 1: _t->on_actionSaveScene_triggered(); break;
        case 2: _t->on_actionNewObjectDatabase_triggered(); break;
        case 3: _t->on_actionLoadObjectDatabase_triggered(); break;
        case 4: _t->on_actionEditObjectDatabase_triggered(); break;
        case 5: _t->on_pb_scene_play_clicked(); break;
        case 6: _t->on_pb_scene_pause_clicked(); break;
        case 7: _t->on_pb_scene_reset_clicked(); break;
        case 8: _t->on_pb_refresh_shaders_clicked(); break;
        case 9: _t->setTime((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 10: _t->savePos(); break;
        case 11: _t->saveRot(); break;
        case 12: _t->saveObjectSpecs(); break;
        case 13: _t->refreshSelected(); break;
        case 14: _t->refreshObjectList(); break;
        case 15: _t->refreshObjectDatabase(); break;
        case 16: _t->on_pb_objects_add_clicked(); break;
        case 17: _t->on_pb_object_remove_clicked(); break;
        case 18: _t->on_lw_objects_active_currentRowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->on_lw_objects_chooser_currentRowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->on_le_obj_pos_x_editingFinished(); break;
        case 21: _t->on_le_obj_pos_y_editingFinished(); break;
        case 22: _t->on_le_obj_pos_z_editingFinished(); break;
        case 23: _t->on_le_obj_rot_x_editingFinished(); break;
        case 24: _t->on_le_obj_rot_y_editingFinished(); break;
        case 25: _t->on_le_obj_rot_z_editingFinished(); break;
        case 26: _t->on_rb_obj_fixed_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 27: _t->on_rb_obj_apply_physic_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: _t->on_rb_obj_physic_static_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 29: _t->on_rb_obj_visible_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 30: _t->on_rb_obj_shadow_only_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 31: _t->saveLightPos(); break;
        case 32: _t->saveLightRot(); break;
        case 33: _t->refreshLightList(); break;
        case 34: _t->on_pb_lights_add_clicked(); break;
        case 35: _t->on_pb_light_remove_clicked(); break;
        case 36: _t->on_lw_lights_active_currentRowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: _t->on_le_light_pos_x_editingFinished(); break;
        case 38: _t->on_le_light_pos_y_editingFinished(); break;
        case 39: _t->on_le_light_pos_z_editingFinished(); break;
        case 40: _t->on_le_light_rot_x_editingFinished(); break;
        case 41: _t->on_le_light_rot_y_editingFinished(); break;
        case 42: _t->on_le_light_rot_z_editingFinished(); break;
        case 43: _t->on_sb_light_r_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 44: _t->on_sb_light_g_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 45: _t->on_sb_light_b_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 46: _t->on_sb_light_intensity_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 47: _t->refreshScripts(); break;
        case 48: _t->refreshScriptElement(); break;
        case 49: _t->on_pb_add_script_clicked(); break;
        case 50: _t->on_pb_remove_script_clicked(); break;
        case 51: _t->on_pb_add_successor_clicked(); break;
        case 52: _t->on_lw_sc_objects_currentRowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 53: _t->on_lw_sc_script_elements_currentRowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 54: _t->on_le_sc_val1_editingFinished(); break;
        case 55: _t->on_le_sc_val2_editingFinished(); break;
        case 56: _t->on_le_sc_val3_editingFinished(); break;
        case 57: _t->on_le_sc_from_editingFinished(); break;
        case 58: _t->on_le_sc_to_editingFinished(); break;
        case 59: _t->on_le_sc_duration_editingFinished(); break;
        case 60: _t->saveScriptElement(); break;
        case 61: _t->on_cb_sc_trigger_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 62: _t->on_cb_shadow_type_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 63: _t->on_cb_shadow_type_point_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 64: _t->on_cb_anti_aliasing_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 65: _t->on_rb_hdr_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 66: _t->on_rb_bloom_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 67: _t->on_rb_ssao_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject PastadEditor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_PastadEditor.data,
      qt_meta_data_PastadEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PastadEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PastadEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PastadEditor.stringdata0))
        return static_cast<void*>(const_cast< PastadEditor*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int PastadEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 68)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 68;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 68)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 68;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
