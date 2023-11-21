select t.member_id as member_id, t.author_id as author_id, round((t.div * 1.0) / (q.s* 1.0), 3) as probability
from (
select c.member_id, b.author_id, COUNT(c.borrowed_id) as div
from books b left join borrowedbooks c on c.book_id = b.book_id
group by c.member_id, b.author_id
) as t left join (
select c.member_id, COUNT(*) as s
from borrowedbooks c
group by c.member_id
) as q on q.member_id = t.member_id
order by t.member_id, t.author_id;