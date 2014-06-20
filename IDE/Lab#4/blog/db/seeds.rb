# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rake db:seed (or created alongside the db with db:setup).
#
# Examples:
#
#   cities = City.create([{ name: 'Chicago' }, { name: 'Copenhagen' }])
#   Mayor.create(name: 'Emanuel', city: cities.first)


posts = [
  {
    title: "Post 1",
    content: "content 1"
  },
  {
    title: "Post 2",
    content: "content 2"
  },
  {
    title: "Post 3",
    content: "content 3"
  },
  {
    title: "Post 4",
    content: "content 4"
  },
]

posts.each do |p|
  Post.create(p)
end
