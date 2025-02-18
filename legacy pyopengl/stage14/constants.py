from config import *

DOOR_FLAG = -1
WALL_TEXTURE_FILENAMES = {
    DOOR_FLAG: "img/doors/door02_7.jpg",
    1: "img/walls/comp1_1.jpg",
    2: "img/walls/tech01_2.jpg",
    3: "img/walls/tech01_7.jpg",
    4: "img/walls/tech08_1.jpg",
    5: "img/walls/tech09_3.jpg",
    6: "img/walls/twall2_6.jpg",
}

FLOOR_TEXTURE_FILENAMES = {
    1: "img/floors/plat_top2.jpg",
    2: "img/floors/sfloor1_2.jpg",
    3: "img/floors/sfloor4_2.jpg",
    4: "img/floors/sfloor4_4.jpg",
    5: "img/floors/sfloor4_6.jpg",
    6: "img/floors/sfloor4_7.jpg",
}

FLOOR_DRAWCALL = np.dtype({
    'names':   [     'i',      'j',  'floor', 'ceiling'], 
    'formats': [np.uint8, np.uint8, np.uint8,  np.uint8],
    'offsets': [       0,        1,        2,         3],
    'itemsize': 4})

WALL_DRAWCALL = np.dtype({
    'names':   [     'i',      'j', 'material',   'mask'], 
    'formats': [np.uint8, np.uint8,    np.int8, np.uint8],
    'offsets': [       0,        1,        2,         3],
    'itemsize': 4})

STATE_NO_CHANGE = 0
STATE_PERSPECTIVE = 1
STATE_TOP_DOWN = 2