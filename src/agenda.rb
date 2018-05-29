# NAME
#   agenda - A much more limited reminder service
#
# DESCRIPTION
# 
#   Like calendar(1) except ...
#
#   - Does not check the current directory for calendarfile
#   - Does not preprocess calendarfile with cpp(1);
#   - Does not use <tab> as separator;
#   - Does not allow other date formats than ISO8601;
#   - Does not jump over weekends

require 'optparse'

VERSION = '0.0.1'
Encoding.default_external = Encoding::UTF_8
Encoding.default_internal = Encoding::UTF_8

class Integer
  def days
    60 * 60 * 24 * self
  end
end

today = Time.new(Time.now.year, Time.now.month, Time.now.day)
advance = 14.days
backward = 0.days
calendarfile = ENV["CALENDAR_FILE"] || "~/CALENDAR"

OptionParser.new do |parser|
  parser.banner = "Usage: #{$PROGRAM_NAME} [arguments]"
  parser.on("-A NUM", "Advance NUM days") do |num|
    advance = num.to_i.days
  end
  parser.on("-B NUM", "Backward NUM days") do |num|
    backward = num.to_i.days
  end
  parser.on("-t DATE", "Adjust today's DATE") do |date|
    today = Time::Format::ISO_8601_DATE.parse(date)
  end
  parser.on("-f FILE", "Use FILE instead of default") do |file|
    calendarfile = file
  end
  parser.on("-v", "--version", "Display version") do
    puts "Agenda #{VERSION}"
    exit
  end
  parser.on("-h", "--help", "Display help") do
    puts parser
    exit
  end
end.parse!

period = today - backward .. today + advance

File.open(File.expand_path(calendarfile), "r").each do |line|
  if line =~ /^\s*(?:(\d{4})-)?(\d{2})-(\d{2})\b/
    date = Time.new(($~[1] || today.year).to_i, $~[2].to_i, $~[3].to_i)
    puts line if period.include? date
  end
end
