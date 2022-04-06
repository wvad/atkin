{
  'targets': [{
    'target_name': 'addon',
    'sources': ['addon.cc'],
    'include_dirs': [
      "<!(node -p \"require('node-addon-api').include_dir\")"
    ],
    'cflags!': ['-fno-exceptions'],
    'cflags_cc!': ['-fno-exceptions', '-std=c++17'],
    'defines': ['NAPI_DISABLE_CPP_EXCEPTIONS'],
  }]
}
