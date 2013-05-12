# -*- mode: ruby; coding: utf-8 -*-

require 'ConstVars'

class RuntimeSettings
  include ConstVars

  def initialize(libdir, domain, dictionary, config, rule)
    @libdir = libdir
    @prefdir = PREF_DIR
    @domain = domain
    @dictionary = dictionary
    @config = config
    @rule = rule
  end

  def pref(path)
    @prefdir + "/" + path
  end

  def lib(path)
    @libdir + "/" + path
  end

  def domain
    pref(@domain)
  end

  def dictionary_set
    lib(DICTIONARY_SET)
  end

  def user_dictionary
    lib(@dictionary)
  end   

  def config
    lib(@config)
  end

  def rule
    lib(@rule)
  end

  attr_writer :libdir, :prefdir
  attr_reader :libdir
end
