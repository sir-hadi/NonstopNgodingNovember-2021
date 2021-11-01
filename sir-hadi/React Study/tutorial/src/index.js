import React from 'react';
import ReactDOM from 'react-dom';
// CSS
import './index.css';

var list = {
  title: 'Way to not give up',
  img: 'https://images-na.ssl-images-amazon.com/images/I/71QKQ9mwV7L._AC_UL200_SR200,200_.jpg',
  author: 'Mark Manson',
};

// we must capitalize the function name, so react will see it
function BookList() {
  return (
    <section className="booklist">
      <Book title={list.title} img={list.img} author={list.author}>
        <p>Hay yo wo</p>
      </Book>
    </section>
  );
}

const Book = ({ title, img, author, children }) => {
  return (
    <article className="book">
      <img src={img} alt="" />
      <h2>{title}</h2>
      <h3 style={{ color: '#617d98', marginTop: '0.25rem' }}>{author}</h3>
      {children}
    </article>
  );
};

ReactDOM.render(<BookList />, document.getElementById('root'));
