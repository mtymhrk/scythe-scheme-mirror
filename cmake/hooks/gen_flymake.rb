#!/usr/bin/env ruby

require 'fileutils'
require 'tempfile'
require 'yaml'

MAKEIFLE = 'Makefile'

START_LINE = '# *** GENERATED BY GEN_FLYMAKE.RB START ***'
END_LINE   = '# *** GENERATED BY GEN_FLYMAKE.RB END *****'

SYNTAX_CHECK_C_FLAGS = ['-fshow-column',
                        '-fno-diagnostics-show-caret',
                        '-fno-diagnostics-show-option']

def makefile_path(builddir, yml_file)
  dir = File.dirname(yml_file).slice((builddir.length + 1)..-1)
  File.join(dir, MAKEIFLE)
end

def replace_file_contents(path)
  return unless block_given?

  file = Tempfile.open(File.basename(path))
  yield(file)
  file.close(false)
  FileUtils.mv(file.path, path)
end

def delete_old_check_syntax_rule(makefile_path)
  return unless File.exist?(makefile_path)

  lines = File.open(makefile_path) { |f| f.readlines }
  replace_file_contents(makefile_path) do |file|
    lines.each do |line|
      l = line.chomp
      file.print(line) unless (l == START_LINE)..(l == END_LINE)
    end
  end
end

def format_syntax_check_c_flags_str
  SYNTAX_CHECK_C_FLAGS.join(' ')
end

def format_compile_c_flags_str(info)
  if info['flags']
    info['flags'].join(' ')
  else
    ''
  end
end

def format_compile_c_ipaths_str(info)
  x = []
  if info['quoted-include-paths']
    x += info['quoted-include-paths'].map { |e| "-iquote #{e}" }
  end

  if info['include-paths']
    x += info['include-paths'].map { |e| "-I#{e}" }
  end

  x.join(' ')
end

def format_compile_c_defines_str(info)
  if info['definitions']
    info['definitions'].map { |x| "-D#{x}" }.join(' ')
  else
    ''
  end
end

def insert_check_syntax_rule(makefile_path, info)
  syntax_check_c_flags_str = format_syntax_check_c_flags_str
  compile_c_flags_str = format_compile_c_flags_str(info)
  compile_c_ipaths_str = format_compile_c_ipaths_str(info)
  compile_c_defines_str = format_compile_c_defines_str(info)

  File.open(makefile_path, 'a+') do |file|
    file.puts(START_LINE)
    file.puts(<<"EOS")

SYNTAX_CHECK_C_FLAGS = #{syntax_check_c_flags_str}
COMPILE_C_FLAGS = #{compile_c_flags_str}
COMPILE_C_IPATHS = #{compile_c_ipaths_str}
COMPILE_C_DEFINES = #{compile_c_defines_str}

check-syntax:
\tgcc ${SYNTAX_CHECK_C_FLAGS} ${COMPILE_C_FLAGS} ${COMPILE_C_IPATHS} ${COMPILE_C_DEFINES} ${CHK_SOURCES} -S -o /dev/null

.PHONY: check-syntax

EOS
    file.puts(END_LINE)
  end
end

if $PROGRAM_NAME == __FILE__
  if ARGV.length == 0
    $stderr.puts("#{$PROGRAM_NAME}: error: too few arguments")
    $stderr.puts("Usage: #{$PROGRAM_NAME} BUILD_DIR_NAME")
    exit 1
  end

  builddir = ARGV[0].sub(/\/*\Z/, '')
  glob = File.join(builddir, '**/build-info.yml')
  Dir[glob].each do |yml_file|
    info = YAML.load(File.open(yml_file).read)
    next unless info

    makefile_path = makefile_path(builddir, yml_file)
    delete_old_check_syntax_rule(makefile_path)
    insert_check_syntax_rule(makefile_path, info)
  end
end