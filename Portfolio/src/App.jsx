import './App.css';

function App() {
  return (
    <>
      <nav className="navbar navbar-expand-lg navbar-dark bg-dark">
        <div className="container">
          <a className="navbar-brand" href="#">My Portfolio</a>
        </div>
      </nav>

      <header className="bg-primary text-white text-center py-5">
        <div className="container">
          <h1>Hello, I'm Akash</h1>
          <p className="lead">Aspiring Developer | MERN & C++ Enthusiast</p>
        </div>
      </header>

      <section className="container my-5">
        <h2 className="text-center mb-4">Projects</h2>
        <div className="row">
          <div className="col-md-6 mb-4">
            <div className="card h-100">
              <div className="card-body">
                <h5 className="card-title">File Encryption Tool (C++)</h5>
                <p className="card-text">A simple XOR-based file encryption and decryption tool written in C++ for securing text files.</p>
                <a href="#" className="btn btn-primary">View on GitHub</a>
              </div>
            </div>
          </div>
          <div className="col-md-6 mb-4">
            <div className="card h-100">
              <div className="card-body">
                <h5 className="card-title">Stopwatch / Clock (C++)</h5>
                <p className="card-text">A CLI-based stopwatch and clock using C++'s time library to measure elapsed time.</p>
                <a href="#" className="btn btn-primary">View on GitHub</a>
              </div>
            </div>
          </div>
        </div>
      </section>

      <section className="bg-light py-5">
        <div className="container">
          <h2 className="text-center mb-4">Contact</h2>
          <div className="text-center">
            <p>Email: akash@example.com</p>
            <p>GitHub: <a href="https://github.com/yourusername" target="_blank">github.com/yourusername</a></p>
          </div>
        </div>
      </section>

      <footer className="bg-dark text-white text-center py-3">
        <p className="mb-0">&copy; 2025 Akash. All rights reserved.</p>
      </footer>
    </>
  );
}

export default App;
