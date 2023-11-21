select a.author_id, a.name, b.book_id, b.title, t.month, COUNT(*)
from authors a natural join books b natural join (
select c.book_id, strftime('%Y', c.borrow_date) || "-" || strftime('%m', c.borrow_date) as month
from borrowedbooks c
) as t
group by a.author_id, a.name, b.book_id, b.title, t.month
order by a.author_id, b.book_id, t.month;
