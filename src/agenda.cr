require "option_parser"

today = Time.now.at_beginning_of_day
advance = 14.days
backward = 0.days
calendarfile = ENV["CALENDAR_FILE"]? || "~/CALENDAR"
editor = ENV["EDITOR"]? || "vi"

OptionParser.parse! do |options|
  options.banner = "Usage: #{PROGRAM_NAME} [arguments]"
  options.on("-A NUM", "Advance NUM days") do |num|
    advance = num.to_i.days
  end
  options.on("-B NUM", "Backward NUM days") do |num|
    backward = num.to_i.days
  end
  options.on("-t DATE", "Adjust today's DATE") do |date|
    today = Time::Format::ISO_8601_DATE.parse(date)
  end
  options.on("-f FILE", "Use FILE instead of default") do |file|
    calendarfile = file
  end
  options.on("-e", "Open calendar file using EDITOR") do
    Process.exec(editor, [calendarfile])
    exit
  end
  options.on("-h", "--help", "Display help") do
    puts options
    exit
  end
end
period = today - backward..today + advance

File.each_line(File.expand_path(calendarfile)) do |line|
  if line =~ /^\s*(?:(\d{4})-)?(\d{2})-(\d{2})\b/
    date = Time.new(($~[1]? || today.year).to_i, $~[2].to_i, $~[3].to_i)
    puts line if period.includes? date
  end
end
