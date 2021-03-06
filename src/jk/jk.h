#ifndef JK_H
#define JK_H

#include <cstddef>
#include <memory> 
#include <vector>
#include <ambit/tensor.h>

namespace lightspeed {

class SBasisSet;
class SchwarzSieve;

enum JKType {
    kBase,
    kDirect,
    kDF,
};

class JK {

public:
    JK(const std::shared_ptr<SchwarzSieve>& sieve);
    virtual ~JK() {}    

    virtual JKType type() const { return kBase; }
    virtual ambit::TensorType tensor_type() const { return ambit::kCore; }

    const std::shared_ptr<SBasisSet>& primary() const { return primary_; }
    const std::shared_ptr<SchwarzSieve>& sieve() const { return sieve_; }
    size_t doubles() const { return doubles_; }
    bool compute_J() const { return compute_J_; }
    bool compute_K() const { return compute_K_; }
    double a() const { return a_; }
    double b() const { return b_; }
    double w() const { return w_; }
    double product_cutoff() const { return product_cutoff_; }

    void set_doubles(size_t doubles) { doubles_ = doubles; }
    void set_compute_J(bool compute_J) { compute_J_ = compute_J; }
    void set_compute_K(bool compute_K) { compute_K_ = compute_K; }
    void set_a(double a) { a_ = a; }
    void set_b(double b) { b_ = b; }
    void set_w(double w) { w_ = w; }
    void set_product_cutoff(double product_cutoff) { product_cutoff_ = product_cutoff; }

    virtual void initialize();
    
    virtual void print(
        FILE* fh = stdout) const;

    virtual void compute_JK_from_C(
        const std::vector<ambit::Tensor>& L,
        const std::vector<ambit::Tensor>& R,
        std::vector<ambit::Tensor>& J,
        std::vector<ambit::Tensor>& K,
        const std::vector<double>& scaleJ = {},
        const std::vector<double>& scaleK = {});

    virtual void compute_JK_from_D(
        const std::vector<ambit::Tensor>& D,
        const std::vector<bool>& symm,
        std::vector<ambit::Tensor>& J,
        std::vector<ambit::Tensor>& K,
        const std::vector<double>& scaleJ = {},
        const std::vector<double>& scaleK = {});

    virtual void finalize();

#if 0

    virtual void compute_JK_grad_from_C(
        const std::vector<ambit::Tensor>& L1,
        const std::vector<ambit::Tensor>& R1,
        const std::vector<ambit::Tensor>& L2,
        const std::vector<ambit::Tensor>& R2,
        std::vector<ambit::Tensor>& J = {},
        std::vector<ambit::Tensor>& K = {},
        const std::vector<double>& scaleJ = {},
        const std::vector<double>& scaleK = {});

    virtual void compute_JK_grad_from_D(
        const std::vector<ambit::Tensor>& D1,
        const std::vector<ambit::Tensor>& D2,
        std::vector<ambit::Tensor>& J = {},
        std::vector<ambit::Tensor>& K = {},
        const std::vector<double>& scaleJ = {},
        const std::vector<double>& scaleK = {});

    virtual void compute_JK_hess_from_C(
        const std::vector<ambit::Tensor>& L1,
        const std::vector<ambit::Tensor>& R1,
        const std::vector<ambit::Tensor>& L2,
        const std::vector<ambit::Tensor>& R2,
        std::vector<ambit::Tensor>& J = {},
        std::vector<ambit::Tensor>& K = {},
        const std::vector<double>& scaleJ = {},
        const std::vector<double>& scaleK = {});

    virtual void compute_JK_hess_from_D(
        const std::vector<ambit::Tensor>& D1,
        const std::vector<ambit::Tensor>& D2,
        std::vector<ambit::Tensor>& J = {},
        std::vector<ambit::Tensor>& K = {},
        const std::vector<double>& scaleJ = {},
        const std::vector<double>& scaleK = {});

#endif

protected:

    std::shared_ptr<SBasisSet> primary_;
    std::shared_ptr<SchwarzSieve> sieve_; 

    size_t doubles_ = 256000000;
    bool compute_J_ = true;
    bool compute_K_ = true;
    double a_ = 1.0;
    double b_ = 0.0;
    double w_ = 0.0;
    double product_cutoff_ = 0.0;

};

class DirectJK final: public JK {

public:
    DirectJK(const std::shared_ptr<SchwarzSieve>& sieve);
    virtual ~DirectJK() override {}    

    JKType type() const override { return kDirect; }
    ambit::TensorType tensor_type() const override { return ambit::kCore; }

    void initialize() override {}
    
    virtual void print(
        FILE* fh = stdout) const override;

    void compute_JK_from_C(
        const std::vector<ambit::Tensor>& L,
        const std::vector<ambit::Tensor>& R,
        std::vector<ambit::Tensor>& J,
        std::vector<ambit::Tensor>& K,
        const std::vector<double>& scaleJ = {},
        const std::vector<double>& scaleK = {}) override;

    void compute_JK_from_D(
        const std::vector<ambit::Tensor>& D,
        const std::vector<bool>& symm,
        std::vector<ambit::Tensor>& J,
        std::vector<ambit::Tensor>& K,
        const std::vector<double>& scaleJ = {},
        const std::vector<double>& scaleK = {}) override;

    void finalize() override {}

};

class DFJK final: public JK {

public:
    DFJK(
        const std::shared_ptr<SchwarzSieve>& sieve,
        const std::shared_ptr<SBasisSet>& auxiliary);
    virtual ~DFJK() override {}    

    JKType type() const override { return kDF; }
    ambit::TensorType tensor_type() const override { return ambit::kCore; }

    double metric_condition() const { return metric_condition_; }
    bool force_disk() const { return force_disk_; }
    void set_metric_condition(double metric_condition) { metric_condition_ = metric_condition; }
    void set_force_disk(bool force_disk) { force_disk_ = force_disk; }

    void initialize() override;
    
    virtual void print(
        FILE* fh = stdout) const;

    void compute_JK_from_C(
        const std::vector<ambit::Tensor>& L,
        const std::vector<ambit::Tensor>& R,
        std::vector<ambit::Tensor>& J,
        std::vector<ambit::Tensor>& K,
        const std::vector<double>& scaleJ = {},
        const std::vector<double>& scaleK = {}) override;

    void compute_JK_from_D(
        const std::vector<ambit::Tensor>& D,
        const std::vector<bool>& symm,
        std::vector<ambit::Tensor>& J,
        std::vector<ambit::Tensor>& K,
        const std::vector<double>& scaleJ = {},
        const std::vector<double>& scaleK = {}) override;

    void finalize() override;

protected:
    
    std::shared_ptr<SBasisSet> auxiliary_;
    double metric_condition_ = 1.0E-12;
    bool force_disk_ = false;

private:
    
    bool is_core_ = true;
    ambit::Tensor df_tensor_;
    bool initialized_ = false;

};

} // namespace lightspeed

#endif

