/****************************************************************************
** Meta object code from reading C++ file 'editorwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qtPastadEditor/editorwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'editorwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EditorWindow_t {
    QByteArrayData data[78];
    char stringdata0[1913];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EditorWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EditorWindow_t qt_meta_stringdata_EditorWindow = {
    {
QT_MOC_LITERAL(0, 0, 12), // "EditorWindow"
QT_MOC_LITERAL(1, 13, 21), // "on_pbNewScene_clicked"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 7), // "checked"
QT_MOC_LITERAL(4, 44, 22), // "on_pbLoadScene_clicked"
QT_MOC_LITERAL(5, 67, 22), // "on_pbSaveScene_clicked"
QT_MOC_LITERAL(6, 90, 24), // "on_pb_scene_play_clicked"
QT_MOC_LITERAL(7, 115, 25), // "on_pb_scene_pause_clicked"
QT_MOC_LITERAL(8, 141, 25), // "on_pb_scene_reset_clicked"
QT_MOC_LITERAL(9, 167, 7), // "savePos"
QT_MOC_LITERAL(10, 175, 7), // "saveRot"
QT_MOC_LITERAL(11, 183, 9), // "saveScale"
QT_MOC_LITERAL(12, 193, 15), // "saveObjectSpecs"
QT_MOC_LITERAL(13, 209, 21), // "refreshObjectDatabase"
QT_MOC_LITERAL(14, 231, 25), // "on_pb_objects_add_clicked"
QT_MOC_LITERAL(15, 257, 27), // "on_pb_object_remove_clicked"
QT_MOC_LITERAL(16, 285, 38), // "on_lw_objects_active_currentR..."
QT_MOC_LITERAL(17, 324, 3), // "row"
QT_MOC_LITERAL(18, 328, 39), // "on_lw_objects_chooser_current..."
QT_MOC_LITERAL(19, 368, 31), // "on_le_obj_pos_x_editingFinished"
QT_MOC_LITERAL(20, 400, 31), // "on_le_obj_pos_y_editingFinished"
QT_MOC_LITERAL(21, 432, 31), // "on_le_obj_pos_z_editingFinished"
QT_MOC_LITERAL(22, 464, 31), // "on_le_obj_rot_x_editingFinished"
QT_MOC_LITERAL(23, 496, 31), // "on_le_obj_rot_y_editingFinished"
QT_MOC_LITERAL(24, 528, 31), // "on_le_obj_rot_z_editingFinished"
QT_MOC_LITERAL(25, 560, 33), // "on_le_obj_scale_x_editingFini..."
QT_MOC_LITERAL(26, 594, 33), // "on_le_obj_scale_y_editingFini..."
QT_MOC_LITERAL(27, 628, 33), // "on_le_obj_scale_z_editingFini..."
QT_MOC_LITERAL(28, 662, 23), // "on_rb_obj_fixed_clicked"
QT_MOC_LITERAL(29, 686, 5), // "value"
QT_MOC_LITERAL(30, 692, 30), // "on_rb_obj_apply_physic_clicked"
QT_MOC_LITERAL(31, 723, 31), // "on_rb_obj_physic_static_clicked"
QT_MOC_LITERAL(32, 755, 25), // "on_rb_obj_visible_clicked"
QT_MOC_LITERAL(33, 781, 29), // "on_rb_obj_shadow_only_clicked"
QT_MOC_LITERAL(34, 811, 37), // "on_cb_shadow_type_currentInde..."
QT_MOC_LITERAL(35, 849, 3), // "idx"
QT_MOC_LITERAL(36, 853, 43), // "on_cb_shadow_type_point_curre..."
QT_MOC_LITERAL(37, 897, 39), // "on_cb_anti_aliasing_currentIn..."
QT_MOC_LITERAL(38, 937, 17), // "on_rb_hdr_clicked"
QT_MOC_LITERAL(39, 955, 19), // "on_rb_bloom_clicked"
QT_MOC_LITERAL(40, 975, 18), // "on_rb_ssao_clicked"
QT_MOC_LITERAL(41, 994, 19), // "on_pbDBLoad_clicked"
QT_MOC_LITERAL(42, 1014, 18), // "on_pbDBNew_clicked"
QT_MOC_LITERAL(43, 1033, 17), // "on_pbEdit_clicked"
QT_MOC_LITERAL(44, 1051, 12), // "saveLightPos"
QT_MOC_LITERAL(45, 1064, 12), // "saveLightRot"
QT_MOC_LITERAL(46, 1077, 24), // "on_pb_lights_add_clicked"
QT_MOC_LITERAL(47, 1102, 26), // "on_pb_light_remove_clicked"
QT_MOC_LITERAL(48, 1129, 37), // "on_lw_lights_active_currentRo..."
QT_MOC_LITERAL(49, 1167, 33), // "on_le_light_pos_x_editingFini..."
QT_MOC_LITERAL(50, 1201, 33), // "on_le_light_pos_y_editingFini..."
QT_MOC_LITERAL(51, 1235, 33), // "on_le_light_pos_z_editingFini..."
QT_MOC_LITERAL(52, 1269, 33), // "on_le_light_rot_x_editingFini..."
QT_MOC_LITERAL(53, 1303, 33), // "on_le_light_rot_y_editingFini..."
QT_MOC_LITERAL(54, 1337, 33), // "on_le_light_rot_z_editingFini..."
QT_MOC_LITERAL(55, 1371, 26), // "on_sb_light_r_valueChanged"
QT_MOC_LITERAL(56, 1398, 1), // "i"
QT_MOC_LITERAL(57, 1400, 26), // "on_sb_light_g_valueChanged"
QT_MOC_LITERAL(58, 1427, 26), // "on_sb_light_b_valueChanged"
QT_MOC_LITERAL(59, 1454, 34), // "on_sb_light_intensity_valueCh..."
QT_MOC_LITERAL(60, 1489, 1), // "d"
QT_MOC_LITERAL(61, 1491, 11), // "changeLight"
QT_MOC_LITERAL(62, 1503, 6), // "Light*"
QT_MOC_LITERAL(63, 1510, 5), // "light"
QT_MOC_LITERAL(64, 1516, 14), // "refreshScripts"
QT_MOC_LITERAL(65, 1531, 20), // "refreshScriptElement"
QT_MOC_LITERAL(66, 1552, 24), // "on_pb_add_script_clicked"
QT_MOC_LITERAL(67, 1577, 27), // "on_pb_remove_script_clicked"
QT_MOC_LITERAL(68, 1605, 27), // "on_pb_add_successor_clicked"
QT_MOC_LITERAL(69, 1633, 42), // "on_lw_sc_script_elements_curr..."
QT_MOC_LITERAL(70, 1676, 29), // "on_le_sc_val1_editingFinished"
QT_MOC_LITERAL(71, 1706, 29), // "on_le_sc_val2_editingFinished"
QT_MOC_LITERAL(72, 1736, 29), // "on_le_sc_val3_editingFinished"
QT_MOC_LITERAL(73, 1766, 29), // "on_le_sc_from_editingFinished"
QT_MOC_LITERAL(74, 1796, 27), // "on_le_sc_to_editingFinished"
QT_MOC_LITERAL(75, 1824, 33), // "on_le_sc_duration_editingFini..."
QT_MOC_LITERAL(76, 1858, 17), // "saveScriptElement"
QT_MOC_LITERAL(77, 1876, 36) // "on_cb_sc_trigger_currentIndex..."

    },
    "EditorWindow\0on_pbNewScene_clicked\0\0"
    "checked\0on_pbLoadScene_clicked\0"
    "on_pbSaveScene_clicked\0on_pb_scene_play_clicked\0"
    "on_pb_scene_pause_clicked\0"
    "on_pb_scene_reset_clicked\0savePos\0"
    "saveRot\0saveScale\0saveObjectSpecs\0"
    "refreshObjectDatabase\0on_pb_objects_add_clicked\0"
    "on_pb_object_remove_clicked\0"
    "on_lw_objects_active_currentRowChanged\0"
    "row\0on_lw_objects_chooser_currentRowChanged\0"
    "on_le_obj_pos_x_editingFinished\0"
    "on_le_obj_pos_y_editingFinished\0"
    "on_le_obj_pos_z_editingFinished\0"
    "on_le_obj_rot_x_editingFinished\0"
    "on_le_obj_rot_y_editingFinished\0"
    "on_le_obj_rot_z_editingFinished\0"
    "on_le_obj_scale_x_editingFinished\0"
    "on_le_obj_scale_y_editingFinished\0"
    "on_le_obj_scale_z_editingFinished\0"
    "on_rb_obj_fixed_clicked\0value\0"
    "on_rb_obj_apply_physic_clicked\0"
    "on_rb_obj_physic_static_clicked\0"
    "on_rb_obj_visible_clicked\0"
    "on_rb_obj_shadow_only_clicked\0"
    "on_cb_shadow_type_currentIndexChanged\0"
    "idx\0on_cb_shadow_type_point_currentIndexChanged\0"
    "on_cb_anti_aliasing_currentIndexChanged\0"
    "on_rb_hdr_clicked\0on_rb_bloom_clicked\0"
    "on_rb_ssao_clicked\0on_pbDBLoad_clicked\0"
    "on_pbDBNew_clicked\0on_pbEdit_clicked\0"
    "saveLightPos\0saveLightRot\0"
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
    "changeLight\0Light*\0light\0refreshScripts\0"
    "refreshScriptElement\0on_pb_add_script_clicked\0"
    "on_pb_remove_script_clicked\0"
    "on_pb_add_successor_clicked\0"
    "on_lw_sc_script_elements_currentRowChanged\0"
    "on_le_sc_val1_editingFinished\0"
    "on_le_sc_val2_editingFinished\0"
    "on_le_sc_val3_editingFinished\0"
    "on_le_sc_from_editingFinished\0"
    "on_le_sc_to_editingFinished\0"
    "on_le_sc_duration_editingFinished\0"
    "saveScriptElement\0"
    "on_cb_sc_trigger_currentIndexChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EditorWindow[] = {

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
       1,    1,  354,    2, 0x08 /* Private */,
       4,    1,  357,    2, 0x08 /* Private */,
       5,    1,  360,    2, 0x08 /* Private */,
       6,    0,  363,    2, 0x08 /* Private */,
       7,    0,  364,    2, 0x08 /* Private */,
       8,    0,  365,    2, 0x08 /* Private */,
       9,    0,  366,    2, 0x08 /* Private */,
      10,    0,  367,    2, 0x08 /* Private */,
      11,    0,  368,    2, 0x08 /* Private */,
      12,    0,  369,    2, 0x08 /* Private */,
      13,    0,  370,    2, 0x08 /* Private */,
      14,    0,  371,    2, 0x08 /* Private */,
      15,    0,  372,    2, 0x08 /* Private */,
      16,    1,  373,    2, 0x08 /* Private */,
      18,    1,  376,    2, 0x08 /* Private */,
      19,    0,  379,    2, 0x08 /* Private */,
      20,    0,  380,    2, 0x08 /* Private */,
      21,    0,  381,    2, 0x08 /* Private */,
      22,    0,  382,    2, 0x08 /* Private */,
      23,    0,  383,    2, 0x08 /* Private */,
      24,    0,  384,    2, 0x08 /* Private */,
      25,    0,  385,    2, 0x08 /* Private */,
      26,    0,  386,    2, 0x08 /* Private */,
      27,    0,  387,    2, 0x08 /* Private */,
      28,    1,  388,    2, 0x08 /* Private */,
      30,    1,  391,    2, 0x08 /* Private */,
      31,    1,  394,    2, 0x08 /* Private */,
      32,    1,  397,    2, 0x08 /* Private */,
      33,    1,  400,    2, 0x08 /* Private */,
      34,    1,  403,    2, 0x08 /* Private */,
      36,    1,  406,    2, 0x08 /* Private */,
      37,    1,  409,    2, 0x08 /* Private */,
      38,    1,  412,    2, 0x08 /* Private */,
      39,    1,  415,    2, 0x08 /* Private */,
      40,    1,  418,    2, 0x08 /* Private */,
      41,    1,  421,    2, 0x08 /* Private */,
      42,    1,  424,    2, 0x08 /* Private */,
      43,    1,  427,    2, 0x08 /* Private */,
      44,    0,  430,    2, 0x08 /* Private */,
      45,    0,  431,    2, 0x08 /* Private */,
      46,    0,  432,    2, 0x08 /* Private */,
      47,    0,  433,    2, 0x08 /* Private */,
      48,    1,  434,    2, 0x08 /* Private */,
      49,    0,  437,    2, 0x08 /* Private */,
      50,    0,  438,    2, 0x08 /* Private */,
      51,    0,  439,    2, 0x08 /* Private */,
      52,    0,  440,    2, 0x08 /* Private */,
      53,    0,  441,    2, 0x08 /* Private */,
      54,    0,  442,    2, 0x08 /* Private */,
      55,    1,  443,    2, 0x08 /* Private */,
      57,    1,  446,    2, 0x08 /* Private */,
      58,    1,  449,    2, 0x08 /* Private */,
      59,    1,  452,    2, 0x08 /* Private */,
      61,    1,  455,    2, 0x08 /* Private */,
      64,    0,  458,    2, 0x08 /* Private */,
      65,    0,  459,    2, 0x08 /* Private */,
      66,    0,  460,    2, 0x08 /* Private */,
      67,    0,  461,    2, 0x08 /* Private */,
      68,    0,  462,    2, 0x08 /* Private */,
      69,    1,  463,    2, 0x08 /* Private */,
      70,    0,  466,    2, 0x08 /* Private */,
      71,    0,  467,    2, 0x08 /* Private */,
      72,    0,  468,    2, 0x08 /* Private */,
      73,    0,  469,    2, 0x08 /* Private */,
      74,    0,  470,    2, 0x08 /* Private */,
      75,    0,  471,    2, 0x08 /* Private */,
      76,    0,  472,    2, 0x08 /* Private */,
      77,    1,  473,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   29,
    QMetaType::Void, QMetaType::Bool,   29,
    QMetaType::Void, QMetaType::Bool,   29,
    QMetaType::Void, QMetaType::Bool,   29,
    QMetaType::Void, QMetaType::Bool,   29,
    QMetaType::Void, QMetaType::Int,   35,
    QMetaType::Void, QMetaType::Int,   35,
    QMetaType::Void, QMetaType::Int,   35,
    QMetaType::Void, QMetaType::Bool,   29,
    QMetaType::Void, QMetaType::Bool,   29,
    QMetaType::Void, QMetaType::Bool,   29,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   56,
    QMetaType::Void, QMetaType::Int,   56,
    QMetaType::Void, QMetaType::Int,   56,
    QMetaType::Void, QMetaType::Double,   60,
    QMetaType::Void, 0x80000000 | 62,   63,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   35,

       0        // eod
};

void EditorWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EditorWindow *_t = static_cast<EditorWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pbNewScene_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->on_pbLoadScene_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_pbSaveScene_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_pb_scene_play_clicked(); break;
        case 4: _t->on_pb_scene_pause_clicked(); break;
        case 5: _t->on_pb_scene_reset_clicked(); break;
        case 6: _t->savePos(); break;
        case 7: _t->saveRot(); break;
        case 8: _t->saveScale(); break;
        case 9: _t->saveObjectSpecs(); break;
        case 10: _t->refreshObjectDatabase(); break;
        case 11: _t->on_pb_objects_add_clicked(); break;
        case 12: _t->on_pb_object_remove_clicked(); break;
        case 13: _t->on_lw_objects_active_currentRowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->on_lw_objects_chooser_currentRowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->on_le_obj_pos_x_editingFinished(); break;
        case 16: _t->on_le_obj_pos_y_editingFinished(); break;
        case 17: _t->on_le_obj_pos_z_editingFinished(); break;
        case 18: _t->on_le_obj_rot_x_editingFinished(); break;
        case 19: _t->on_le_obj_rot_y_editingFinished(); break;
        case 20: _t->on_le_obj_rot_z_editingFinished(); break;
        case 21: _t->on_le_obj_scale_x_editingFinished(); break;
        case 22: _t->on_le_obj_scale_y_editingFinished(); break;
        case 23: _t->on_le_obj_scale_z_editingFinished(); break;
        case 24: _t->on_rb_obj_fixed_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: _t->on_rb_obj_apply_physic_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 26: _t->on_rb_obj_physic_static_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 27: _t->on_rb_obj_visible_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: _t->on_rb_obj_shadow_only_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 29: _t->on_cb_shadow_type_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->on_cb_shadow_type_point_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->on_cb_anti_aliasing_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->on_rb_hdr_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 33: _t->on_rb_bloom_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 34: _t->on_rb_ssao_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 35: _t->on_pbDBLoad_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 36: _t->on_pbDBNew_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 37: _t->on_pbEdit_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 38: _t->saveLightPos(); break;
        case 39: _t->saveLightRot(); break;
        case 40: _t->on_pb_lights_add_clicked(); break;
        case 41: _t->on_pb_light_remove_clicked(); break;
        case 42: _t->on_lw_lights_active_currentRowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 43: _t->on_le_light_pos_x_editingFinished(); break;
        case 44: _t->on_le_light_pos_y_editingFinished(); break;
        case 45: _t->on_le_light_pos_z_editingFinished(); break;
        case 46: _t->on_le_light_rot_x_editingFinished(); break;
        case 47: _t->on_le_light_rot_y_editingFinished(); break;
        case 48: _t->on_le_light_rot_z_editingFinished(); break;
        case 49: _t->on_sb_light_r_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 50: _t->on_sb_light_g_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 51: _t->on_sb_light_b_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 52: _t->on_sb_light_intensity_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 53: _t->changeLight((*reinterpret_cast< Light*(*)>(_a[1]))); break;
        case 54: _t->refreshScripts(); break;
        case 55: _t->refreshScriptElement(); break;
        case 56: _t->on_pb_add_script_clicked(); break;
        case 57: _t->on_pb_remove_script_clicked(); break;
        case 58: _t->on_pb_add_successor_clicked(); break;
        case 59: _t->on_lw_sc_script_elements_currentRowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 60: _t->on_le_sc_val1_editingFinished(); break;
        case 61: _t->on_le_sc_val2_editingFinished(); break;
        case 62: _t->on_le_sc_val3_editingFinished(); break;
        case 63: _t->on_le_sc_from_editingFinished(); break;
        case 64: _t->on_le_sc_to_editingFinished(); break;
        case 65: _t->on_le_sc_duration_editingFinished(); break;
        case 66: _t->saveScriptElement(); break;
        case 67: _t->on_cb_sc_trigger_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject EditorWindow::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_EditorWindow.data,
      qt_meta_data_EditorWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *EditorWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EditorWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_EditorWindow.stringdata0))
        return static_cast<void*>(const_cast< EditorWindow*>(this));
    return QDialog::qt_metacast(_clname);
}

int EditorWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
