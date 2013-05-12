# -*- mode: ruby; coding: utf-8 -*-

module ConstVars
  INSTALL_DIR = '/Library/Input Methods'
  BUNDLE_DIR = INSTALL_DIR + '/AquaSKK.app'
  BUNDLE_EXE = BUNDLE_DIR + '/Contents/MacOS/AquaSKK'

  PREF_DIR = File.expand_path("~/Library/Preferences")

  OLD_LIB_DIR = File.expand_path("~/Library/AquaSKK")
  NEW_LIB_DIR = File.expand_path("~/Library/Application Support/AquaSKK")
  TEMP_DIR = "/tmp/setup.aquaskk"

  DICTIONARY_SET = "DictionarySet.plist"
end
